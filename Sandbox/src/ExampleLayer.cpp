#include "ExampleLayer.h"

using namespace PKE;

ExampleSandboxLayer::ExampleSandboxLayer() : m_CameraController(16.0f/9.0f, 1.0f, true)
{
    auto& application = Application::Get();
        
    // Bind Inputs to do some logic
        
    // Square Translation Controller
    auto squareTranslateController = new Standard2DInputController([this](glm::vec2 input)
    {
        m_SquareTransform.Move(glm::vec3(input.x * Time::deltaTime, input.y * Time::deltaTime, 0));
    }, KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow);
    squareTranslateController->SetSpeed(1.f);
    application.PushOverlay(squareTranslateController);
        
    // Square Scale Controller
    auto squareScaleController = new Standard2DInputController([this](glm::vec2 input)
    {
        glm::vec3 scale = m_SquareTransform.GetScale() + glm::vec3(input.x * Time::deltaTime);
        m_SquareTransform.SetScale(scale);
    }, KeyCode::Keypad0, KeyCode::Keypad1, KeyCode::None, KeyCode::None);
    squareScaleController->SetSpeed(1.f);
    application.PushOverlay(squareScaleController);

    // -- TRIANGLE --
        
    // Store indices and vertices coordinates for the triangle.
    uint32_t indices[3] = {0, 1, 2};
    float triangleVertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,// 1.0f, 1.0f, 0.f, 0.f,
        0.5f, -0.5f, 0.0f,// 1.0f, 0.0f, 0.f, 0.f,
        0.0f, 0.5f, 0.0f,// 1.0f, 0.0f, 1.f, 0.f,
    };

    BufferLayout triangleLayout = {
        {ShaderDataType::Float3, "a_Position"},
        // {ShaderDataType::Float4, "a_Color"},
    };

    Ref<VertexBuffer> triangleVB = PKE::Ref<VertexBuffer>(
        VertexBuffer::Create(triangleVertices, sizeof(triangleVertices))
    );
    triangleVB->SetLayout(triangleLayout);
    Ref<IndexBuffer> triangleIB = PKE::Ref<IndexBuffer>(
        IndexBuffer::Create(indices, 3)
    );
        
    m_TriangleVertexArray.reset(VertexArray::Create());
    m_TriangleVertexArray->AddVertexBuffer(triangleVB);
    m_TriangleVertexArray->SetIndexBuffer(triangleIB);

    // -- SQUARE --
        
    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    float squareVertices[4 * 7] = {
        -0.25f, -0.25f, 0.0f,   0.0f, 0.0f,// 1.f, 1.f,
        0.25f, -0.25f, 0.0f,    1.0f, 0.0f,// 1.f, 1.f,
        0.25f, 0.25f, 0.0f,     1.0f, 1.0f,// 0.f, 1.f,
        -0.25f, 0.25f, 0.0f,    0.0f, 1.0f,// 0.f, 1.f,
    };
        
    BufferLayout squareLayout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float2, "a_TexCoord"},
    };

    Ref<VertexBuffer> squareVB = PKE::Ref<VertexBuffer>(
        VertexBuffer::Create(squareVertices, sizeof(squareVertices))
    );
    squareVB->SetLayout(squareLayout);
    Ref<IndexBuffer> squareIB = PKE::Ref<IndexBuffer>(
        IndexBuffer::Create(squareIndices, 6)
    );
        
    m_SquareVertexArray.reset(VertexArray::Create());
    m_SquareVertexArray->AddVertexBuffer(squareVB);
    m_SquareVertexArray->SetIndexBuffer(squareIB);
        
    // Load Textures
    m_RazowskiTexture = ToRef(Texture2D::Create("assets/textures/razowski.png"));
    m_LoveTexture = ToRef(Texture2D::Create("assets/textures/love.png"));
    m_CppTexture = ToRef(Texture2D::Create("assets/textures/cpp.png"));

    Ref<Shader> textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
    textureShader->Bind();
    textureShader->SetUniformInt("u_Texture", 0);
        
    m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
    m_ShaderLibrary.Load("assets/shaders/WorldPositionColor.glsl");
    m_ShaderLibrary.Load("assets/shaders/ScreenPositionColor.glsl");
        
    // m_FlatColorShader.reset(Shader::MakeFlatColorShader());
    // m_PositionColorShader.reset(Shader::MakeScreenPositionColorShader());
        
    EventAction TEST_ACTION;
    auto id = TEST_ACTION.AddListener([] { PKE_LOG_INFO("[T] TEST_ACTION invoked!"); });
    TEST_ACTION.AddListener([] { PKE_LOG_INFO("[Y] TEST_ACTION invoked!"); });
    auto id2 = TEST_ACTION += PKE_BIND_FUNCTION(ExampleSandboxLayer::OnTestActionInvoked);
    TEST_ACTION.Invoke();
    TEST_ACTION.RemoveListener(id);
    TEST_ACTION -= id2;
    TEST_ACTION.Invoke();
    TEST_ACTION.ClearListeners();
    TEST_ACTION.Invoke();
        
    EventAction<float> TEST_ACTION2;
    TEST_ACTION2.AddListener([](float x) { PKE_LOG_INFO("[T] TEST_ACTION2 invoked with value: {}", x); });
    TEST_ACTION2.Invoke(1.2f);
}

void ExampleSandboxLayer::OnTestActionInvoked() { PKE_LOG_INFO("TestAction invoked to func!"); }

void ExampleSandboxLayer::OnImGuiRender()
{
    static bool showParameters = true;
    if (ImGui::Begin("Parameters", &showParameters))
    {
        if (ImGui::CollapsingHeader("Camera Controller", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static constexpr std::array<const char*, 3> aspectRatioOptions = {"None", "Match Width", "Match Height"};
            ImGuiUtility::EnumCombo("Camera Auto Adjust Aspect Ratio", m_CameraController.AspectRatioAdjustment, aspectRatioOptions);
            ImGui::DragFloat("CameraZoom", m_CameraController.GetCamera().GetZoomPtr(), 0.1f, 0.1f, 10.f);
        }
        if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit3("Main Color", glm::value_ptr(MainColor));
        }
    }
    ImGui::End();
        
    static bool showOtherStatistics = true;
    if (ImGui::Begin("Properties", &showOtherStatistics))
    {
        auto camPos = m_CameraController.GetCamera().GetPosition();
        ImGui::Text("Camera Position: x: %.2f y: %.2f z: %.2f", camPos.x, camPos.y, camPos.z);
        ImGui::Text("Camera Rotation: z: %.2f", m_CameraController.GetCamera().GetZRotation());
            
        ImGui::Separator();
            
        auto squarePos = m_SquareTransform.GetPosition();
        auto squareScale = m_SquareTransform.GetScale();
        ImGui::Text("Square Position: x: %.2f y: %.2f z: %.2f", squarePos.x, squarePos.y, squarePos.z);
        ImGui::Text("Square Scale: x: %.2f y: %.2f z: %.2f", squareScale.x, squareScale.y, squareScale.z);
    }
    ImGui::End();
        
    // ImGui::Begin("Hello, I'm a window!", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
    // {
    //     ImGui::Text("This is some text in the window.");
    //     static bool showBelowTest = false;
    //     ImGui::Checkbox("Demo Window", &showBelowTest);
    //
    //     if (showBelowTest)
    //     {
    //         static ImColor testColor = ImColor(0.4f, 0.7f, 0.0f, 1.0f);
    //         ImGui::ColorButton("Test Color Button", ImVec4(0.4f, 0.7f, 0.0f, 1.0f));
    //         ImGui::ColorPicker3("Test Color Picker", (float*)&testColor);
    //         ImGui::TextColored(testColor, "WOW!");
    //         if (ImGui::ArrowButton("Test Arrow Button", ImGuiDir_Right))
    //         {
    //             showWindow = !showWindow;
    //             PKE_LOG_INFO("Arrow button clicked! {}", showWindow);
    //         }
    //     }
    // }
    // ImGui::End();
}

void ExampleSandboxLayer::OnUpdate()
{
    m_CameraController.OnUpdate();
        
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RenderCommand::Clear();
        
    Renderer::BeginScene(m_CameraController.GetCamera());
        
    m_RazowskiTexture->Bind();
    Renderer::SubmitGeometry(m_SquareVertexArray, m_ShaderLibrary.Get("Texture"));
        
    m_CppTexture->Bind();
    Renderer::SubmitGeometry(m_SquareVertexArray, m_ShaderLibrary.Get("Texture"), m_SquareTransform.GetTransformMatrix());
        
    Renderer::SubmitGeometry(m_TriangleVertexArray, m_ShaderLibrary.Get("WorldPositionColor"));
        
    // Renderer::EndScene();
    // return;
        
    glm::vec4 redColor = {1.f, 0.f, 0.f, 1.f};
    glm::vec4 greenColor = {0.f, 1.f, 0.f, 1.f};
        
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        
    static glm::vec4 color = {1.f, 0.f, 0.f, 1.f};
        
    float triCount = 8;
    float speed = 2;

    Ref<Shader> flatColorShader = m_ShaderLibrary.Get("FlatColor");
        
    for (int i = 0; i < triCount; i++)
    {
        if (i % 2 == 0)
        {
            flatColorShader->SetUniformFloat4("u_Color", MainColor / (i*0.2f+1));
        }
        else
        {
            flatColorShader->SetUniformFloat4("u_Color", greenColor / (i*0.2f+1));
        }
        glm::vec3 position = {
            glm::sin(Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
            glm::cos(Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
            0
        };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            // * glm::rotate(glm::mat4(1.0f), -(float)PKE::Time::time*speed - 2*glm::pi<float>() * (i/triCount), glm::vec3(0, 0, 1))
            * scale;
        Renderer::SubmitGeometry(m_TriangleVertexArray, flatColorShader, transform);
    }
        
    Renderer::EndScene();
}

void ExampleSandboxLayer::OnEvent(Event& e)
{
    // PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
        
    m_CameraController.OnEvent(e);
        
    EventDispatcher dispatcher(e);
        
    // Switch VSync when tapping the Spacebar.
    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) {
        if (!e.IsRepeat() && e.GetKeyCode() == KeyCode::Space)
        {
            const auto& application = Application::Get();
            application
                .GetWindow()
                .SetVSync(!application.GetWindow().IsVSync());
        }
            
        return false;
    });
}
