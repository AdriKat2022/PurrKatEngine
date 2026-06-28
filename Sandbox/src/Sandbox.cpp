#include <PurrKatEngine.h>
#include "imgui/imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"

using namespace PKE;

class ExampleSandboxLayer : public ImGuiLayer
{
    
public:
    glm::vec4 MainColor = {1.f, 1.f, 1.f, 1.f};
    
    ExampleSandboxLayer() :
        m_Camera(std::make_shared<OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)) // 16:9 Aspect ratio
    {
        auto& application = Application::Get();
        
        // Bind Inputs to do some logic
        
        // Camera Rotation Controller
        auto cameraRotationController = new Standard2DInputController([this](float x, float _)
        {
            m_Camera->SetZRotation(m_Camera->GetZRotation() + x * Time::deltaTime);
        }, KeyCode::Q, KeyCode::E, KeyCode::None, KeyCode::None);
        cameraRotationController->SetSpeed(1.f);
        application.PushOverlay(cameraRotationController);
        
        // Camera Translation Controller
        auto cameraTranslateController = new Standard2DInputController([this](float x, float y)
        {
            auto camPos = m_Camera->GetPosition();
            camPos.x += x * Time::deltaTime;
            camPos.y += y * Time::deltaTime;
            m_Camera->SetPosition(camPos);
        });
        cameraTranslateController->SetSpeed(1.f);
        application.PushOverlay(cameraTranslateController);
        
        // Square Translation Controller
        auto squareTranslateController = new Standard2DInputController([this](float x, float y)
        {
            m_SquareTransform.Move(glm::vec3(x * Time::deltaTime, y * Time::deltaTime, 0));
        }, KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow);
        squareTranslateController->SetSpeed(1.f);
        application.PushOverlay(squareTranslateController);
        
        // Square Scale Controller
        auto squareScaleController = new Standard2DInputController([this](float x, float _)
        {
            glm::vec3 scale = m_SquareTransform.GetScale() + glm::vec3(x * Time::deltaTime);
            m_SquareTransform.SetScale(scale);
        }, KeyCode::Keypad0, KeyCode::Keypad1, KeyCode::None, KeyCode::None);
        squareScaleController->SetSpeed(1.f);
        application.PushOverlay(squareScaleController);

        // -- TRIANGLE --
        
        // Store indices and vertices coordinates for the triangle.
        uint32_t indices[3] = {0, 1, 2};
        float triangleVertices[3 * 7] = {
            -0.15f, -0.15f, 0.0f, 1.0f, 1.0f, 0.f, 0.f,
            0.15f, -0.15f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.0f, 0.15f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
        };

        BufferLayout triangleLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
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
        
        m_TextureShader.reset(Shader::MakeTextureShader());
        m_Texture = Texture2D::Create("assets/razowski.png");
        m_TextureShader->Bind();
        m_TextureShader->UploadUniformInt("u_Texture", 0);
        
        // m_FlatColorShader.reset(Shader::MakeFlatColorShader());
        // m_PositionColorShader.reset(Shader::MakeScreenPositionColorShader());
        
    }

    void OnImGuiRender() override
    {
        
        ImGui::Begin("Parameters");
        {
            ImGui::ColorEdit3("Main Color", glm::value_ptr(MainColor));
        }
        ImGui::End();
        
        static bool showOtherStatistics = true;
        ImGui::Begin("Properties", &showOtherStatistics);
        {
            auto camPos = m_Camera->GetPosition();
            ImGui::Text("Camera Position: x: %.2f y: %.2f z: %.2f", camPos.x, camPos.y, camPos.z);
            ImGui::Text("Camera Rotation: z: %.2f", m_Camera->GetZRotation());
            
            ImGui::Separator();
            
            auto squarePos = m_SquareTransform.GetPosition();
            auto squareScale = m_SquareTransform.GetScale();
            ImGui::Text("Square Position: x: %.2f y: %.2f z: %.2f", squarePos.x, squarePos.y, squarePos.z);
            ImGui::Text("Square Scale: x: %.2f y: %.2f z: %.2f", squareScale.x, squareScale.y, squareScale.z);
        }
        ImGui::End();
        
        static bool showStatisticsWindow = true;
        ImGui::Begin("Statistics", &showStatisticsWindow);
        {
            ImGui::Text("Framerate: %.2f", 1/Time::deltaTime);
            ImGui::Text("VSync: %d", Application::Get().GetWindow().IsVSync());
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

    void OnUpdate() override
    {
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();
        
        Renderer::BeginScene(*m_Camera);
        
        m_Texture->Bind();
        Renderer::SubmitGeometry(m_SquareVertexArray, m_TextureShader, m_SquareTransform.GetTransformMatrix());
        // Renderer::SubmitGeometry(m_TriangleVertexArray, m_PositionColorShader);
        Renderer::EndScene();
        return;
        
        glm::vec4 redColor = {1.f, 0.f, 0.f, 1.f};
        glm::vec4 greenColor = {0.f, 1.f, 0.f, 1.f};
        
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        
        static glm::vec4 color = {1.f, 0.f, 0.f, 1.f};
        
        float triCount = 8;
        float speed = 2;
        if (false)
        for (int i = 0; i < triCount; i++)
        {
            if (i % 2 == 0)
            {
                m_FlatColorShader->UploadUniformFloat4("u_Color", MainColor / (float)(i+1));
            }
            else
            {
                m_FlatColorShader->UploadUniformFloat4("u_Color", greenColor / (float)(i+1));
            }
            glm::vec3 position = {
                glm::sin(Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
                glm::cos(Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
                0
            };
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            // * glm::rotate(glm::mat4(1.0f), -(float)PKE::Time::time*speed - 2*glm::pi<float>() * (i/triCount), glm::vec3(0, 0, 1))
            * scale;
            Renderer::SubmitGeometry(m_TriangleVertexArray, m_FlatColorShader, transform);
        }
        
        Renderer::EndScene();
    }

    void OnEvent(Event& e) override
    {
        // PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
        
        EventDispatcher dispatcher(e);
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
    
private:
    Ref<Texture2D> m_Texture;
    
    Ref<OrthographicCamera> m_Camera;
    Ref<Shader> m_FlatColorShader;
    Ref<Shader> m_PositionColorShader;
    Ref<Shader> m_TextureShader;
    Ref<VertexArray> m_TriangleVertexArray;
    Ref<VertexArray> m_SquareVertexArray;

    Transform m_SquareTransform;
};

class Sandbox : public Application
{
public:
    Sandbox()
    {
        PKE_LOG_TRACE("Sandbox application start. Hey, that's me! A log from the client!");
        PushLayer(new ExampleSandboxLayer());
        
        // GetWindow().SetVSync(false);
    }
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
