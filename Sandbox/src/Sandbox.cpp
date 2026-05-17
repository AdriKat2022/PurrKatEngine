#include <PurrKatEngine.h>
#include "imgui/imgui.h"

class ExampleSandboxLayer : public PurrKatEngine::ImGuiLayer
{
    
public:
    ExampleSandboxLayer() : m_Camera(std::make_shared<PurrKatEngine::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)) // 16:9 Aspect ratio
    {
        auto standardInputController = new PurrKatEngine::StandardInputController([this](float x, float y)
        {
            auto camPos = m_Camera->GetPosition();
            camPos.x += x;
            camPos.y += y;
            m_Camera->SetPosition(camPos);
        });
        standardInputController->SetSpeed(0.01f);
        PurrKatEngine::Application::Get().PushOverlay(standardInputController);

        // -- TRIANGLE --
        
        // Store indices and vertices coordinates for the triangle.
        uint32_t indices[3] = {0, 1, 2};
        float triangleVertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
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
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.f, 1.f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.f, 1.f,
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.f, 1.f,
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.f, 1.f,
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
out vec3 v_Position;
out vec4 v_Color;
void main()
{
    v_Color = a_Color;
    v_Position = a_Position;
    gl_Position = u_ViewProjection * vec4(a_Position*2, 1.0);
}
)";

        std::string fragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
    color = v_Color;
    // color = vec4(v_Position + vec3(0.5, 0.5, 0), 1.0);
}
)";
        
        m_Shader.reset(PurrKatEngine::Shader::Create(vertexSrc, fragmentSrc));
    }

    void OnImGuiRender() override
    {
        static bool showWindow = true;
        ImGui::Begin("Hello, I'm a window!", &showWindow, ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImGui::Text("Framerate: %.2f", 1/PurrKatEngine::Time::deltaTime);
            ImGui::Text("This is some text in the window.");
            static bool showBelowTest = false;
            ImGui::Checkbox("Demo Window", &showBelowTest);

            if (showBelowTest)
            {
                static ImColor testColor = ImColor(0.4f, 0.7f, 0.0f, 1.0f);
                ImGui::ColorButton("Test Color Button", ImVec4(0.4f, 0.7f, 0.0f, 1.0f));
                ImGui::ColorPicker3("Test Color Picker", (float*)&testColor);
                ImGui::TextColored(testColor, "WOW!");
                if (ImGui::ArrowButton("Test Arrow Button", ImGuiDir_Right))
                {
                    showWindow = !showWindow;
                    PKE_LOG_INFO("Arrow button clicked! {}", showWindow);
                }
            }
        }
        ImGui::End();
    }

    void OnUpdate() override
    {
        PurrKatEngine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        PurrKatEngine::RenderCommand::Clear();
        
        m_Camera->SetRotation(PurrKatEngine::Time::time * 1.f);
        
        PurrKatEngine::Renderer::BeginScene(*m_Camera);
        PurrKatEngine::Renderer::SubmitGeometry(m_SquareVertexArray, m_Shader);
        PurrKatEngine::Renderer::SubmitGeometry(m_TriangleVertexArray, m_Shader);
        PurrKatEngine::Renderer::EndScene();
    }

    void OnEvent(PurrKatEngine::Event& e) override
    {
        // PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
    }
    
private:
    std::shared_ptr<PurrKatEngine::OrthographicCamera> m_Camera;
    std::shared_ptr<PurrKatEngine::Shader> m_Shader;
    std::shared_ptr<PurrKatEngine::VertexArray> m_TriangleVertexArray;
    std::shared_ptr<PurrKatEngine::VertexArray> m_SquareVertexArray;
};

class Sandbox : public PurrKatEngine::Application
{
public:
    Sandbox()
    {
        PurrKatEngine::Log::LogTrace("Sandbox application start. Hey, that's me! A log from the client!");
        PushLayer(new ExampleSandboxLayer());
    }
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
