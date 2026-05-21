#include <PurrKatEngine.h>
#include "imgui/imgui.h"

class ExampleSandboxLayer : public PurrKatEngine::ImGuiLayer
{
    
public:
    ExampleSandboxLayer() : m_Camera(std::make_shared<PurrKatEngine::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)) // 16:9 Aspect ratio
    {
        auto& application = PurrKatEngine::Application::Get();
        
        auto cameraRotationController = new PurrKatEngine::Standard2DInputController([this](float x, float _)
        {
            m_Camera->SetZRotation(m_Camera->GetZRotation() + x * PurrKatEngine::Time::deltaTime);
        }, PurrKatEngine::KeyCode::Q, PurrKatEngine::KeyCode::E, PurrKatEngine::KeyCode::None, PurrKatEngine::KeyCode::None);
        cameraRotationController->SetSpeed(1.f);
        application.PushOverlay(cameraRotationController);
        
        auto cameraTranslateController = new PurrKatEngine::Standard2DInputController([this](float x, float y)
        {
            auto camPos = m_Camera->GetPosition();
            camPos.x += x * PurrKatEngine::Time::deltaTime;
            camPos.y += y * PurrKatEngine::Time::deltaTime;
            m_Camera->SetPosition(camPos);
        });
        cameraTranslateController->SetSpeed(1.f);
        application.PushOverlay(cameraTranslateController);
        
        auto squareTranslateController = new PurrKatEngine::Standard2DInputController([this](float x, float y)
        {
            m_SquareTransform.Move(glm::vec3(x * PurrKatEngine::Time::deltaTime, y * PurrKatEngine::Time::deltaTime, 0));
        }, PurrKatEngine::KeyCode::LeftArrow, PurrKatEngine::KeyCode::RightArrow, PurrKatEngine::KeyCode::DownArrow, PurrKatEngine::KeyCode::UpArrow);
        squareTranslateController->SetSpeed(1.f);
        application.PushOverlay(squareTranslateController);
        
        auto squareScaleController = new PurrKatEngine::Standard2DInputController([this](float x, float _)
        {
            glm::vec3 scale = m_SquareTransform.GetScale() + glm::vec3(x * PurrKatEngine::Time::deltaTime);
            m_SquareTransform.SetScale(scale);
        }, PurrKatEngine::KeyCode::Keypad0, PurrKatEngine::KeyCode::Keypad1, PurrKatEngine::KeyCode::None, PurrKatEngine::KeyCode::None);
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

        PurrKatEngine::BufferLayout triangleLayout = {
            {PurrKatEngine::ShaderDataType::Float3, "a_Position"},
            {PurrKatEngine::ShaderDataType::Float4, "a_Color"},
        };

        std::shared_ptr<PurrKatEngine::VertexBuffer> triangleVB = std::shared_ptr<PurrKatEngine::VertexBuffer>(
            PurrKatEngine::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices))
        );
        triangleVB->SetLayout(triangleLayout);
        std::shared_ptr<PurrKatEngine::IndexBuffer> triangleIB = std::shared_ptr<PurrKatEngine::IndexBuffer>(
            PurrKatEngine::IndexBuffer::Create(indices, 3)
            );
        
        m_TriangleVertexArray.reset(PurrKatEngine::VertexArray::Create());
        m_TriangleVertexArray->AddVertexBuffer(triangleVB);
        m_TriangleVertexArray->SetIndexBuffer(triangleIB);

        // -- SQUARE --
        
        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        float squareVertices[4 * 7] = {
            -0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.f, 1.f,
            0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 1.f, 1.f,
            0.25f, 0.25f, 0.0f, 0.0f, 1.0f, 0.f, 1.f,
            -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.f, 1.f,
        };
        
        PurrKatEngine::BufferLayout squareLayout = {
            {PurrKatEngine::ShaderDataType::Float3, "a_Position"},
            {PurrKatEngine::ShaderDataType::Float4, "a_Color"},
        };

        std::shared_ptr<PurrKatEngine::VertexBuffer> squareVB = std::shared_ptr<PurrKatEngine::VertexBuffer>(
            PurrKatEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices))
            );
        squareVB->SetLayout(squareLayout);
        std::shared_ptr<PurrKatEngine::IndexBuffer> squareIB = std::shared_ptr<PurrKatEngine::IndexBuffer>(
            PurrKatEngine::IndexBuffer::Create(squareIndices, 6)
            );
        
        m_SquareVertexArray.reset(PurrKatEngine::VertexArray::Create());
        m_SquareVertexArray->AddVertexBuffer(squareVB);
        m_SquareVertexArray->SetIndexBuffer(squareIB);
        
        // Test shader code.
        std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
out vec3 v_Position;
out vec4 v_Color;
void main()
{
    v_Color = a_Color;

    v_Position = a_Position;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position*2, 1.0);
}
)";

        std::string fragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
)";
        
        m_FlatColorShader.reset(PurrKatEngine::Shader::Create(vertexSrc, fragmentSrc));
    }

    void OnImGuiRender() override
    {
        
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
            ImGui::Text("Framerate: %.2f", 1/PurrKatEngine::Time::deltaTime);
            ImGui::Text("VSync: %d", PurrKatEngine::Application::Get().GetWindow().IsVSync());
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
        PurrKatEngine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        PurrKatEngine::RenderCommand::Clear();
        
        // m_Camera->SetRotation(PurrKatEngine::Time::time * 1.f);
        
        PurrKatEngine::Renderer::BeginScene(*m_Camera);
        PurrKatEngine::Renderer::SubmitGeometry(m_SquareVertexArray, m_FlatColorShader, m_SquareTransform.GetTransformMatrix());
        PurrKatEngine::Renderer::SubmitGeometry(m_TriangleVertexArray, m_FlatColorShader);
        
        glm::vec4 redColor = {1.f, 0.f, 0.f, 1.f};
        glm::vec4 greenColor = {0.f, 1.f, 0.f, 1.f};
        
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        
        float triCount = 8;
        float speed = 2;
        for (int i = 0; i < triCount; i++)
        {
            if (i % 2 == 0)
            {
                m_FlatColorShader->UploadUniformFloat4("u_Color", redColor / (float)(i+1));
            }
            else
            {
                m_FlatColorShader->UploadUniformFloat4("u_Color", greenColor / (float)(i+1));
            }
            glm::vec3 position = {
                glm::sin(PurrKatEngine::Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
                glm::cos(PurrKatEngine::Time::time*speed + 2*glm::pi<float>() * i/triCount) * (i/triCount),
                0
            };
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            // * glm::rotate(glm::mat4(1.0f), -(float)PurrKatEngine::Time::time*speed - 2*glm::pi<float>() * (i/triCount), glm::vec3(0, 0, 1))
            * scale;
            PurrKatEngine::Renderer::SubmitGeometry(m_TriangleVertexArray, m_FlatColorShader, transform);
        }
        
        PurrKatEngine::Renderer::EndScene();
    }

    void OnEvent(PurrKatEngine::Event& e) override
    {
        // PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
        
        PurrKatEngine::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<PurrKatEngine::KeyPressedEvent>([this](PurrKatEngine::KeyPressedEvent& e) {
            if (!e.IsRepeat() && e.GetKeyCode() == PurrKatEngine::KeyCode::Space)
            {
                const auto& application = PurrKatEngine::Application::Get();
                application
                    .GetWindow()
                    .SetVSync(!application.GetWindow().IsVSync());
            }
            
            return false;
        });
    }
    
private:
    std::shared_ptr<PurrKatEngine::OrthographicCamera> m_Camera;
    std::shared_ptr<PurrKatEngine::Shader> m_FlatColorShader;
    std::shared_ptr<PurrKatEngine::VertexArray> m_TriangleVertexArray;
    std::shared_ptr<PurrKatEngine::VertexArray> m_SquareVertexArray;

    PurrKatEngine::Transform m_SquareTransform;
};

class Sandbox : public PurrKatEngine::Application
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
