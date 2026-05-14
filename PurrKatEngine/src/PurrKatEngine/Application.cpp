#include "pkepch.h"

#include "Application.h"

#include "Components/StandardInputController.h"
#include "Inputs/Input.h"
#include "Inputs/Time.h"
#include "Logs/InternalLog.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    
    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) // 16:9 Aspect ratio
    {
        PKE_CORE_ASSERT(s_Instance == nullptr, "An application already exists.")
        s_Instance = this;
        
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(PKE_BIND_FUNCTION(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        m_TimeManagerLayer = new TimeManagerLayer();
        PushOverlay(m_TimeManagerLayer);
        auto standardInputController = new StandardInputController([this](float x, float y)
        {
            auto camPos = m_Camera.GetPosition();
            camPos.x += x;
            camPos.y += y;
            m_Camera.SetPosition(camPos);
        });
        standardInputController->SetSpeed(0.01f);
        PushOverlay(standardInputController);

        m_TriangleVertexArray.reset(VertexArray::Create());
        
        // Create points (vertices) and store them into the vertex array buffer.
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
        };

        std::shared_ptr<VertexBuffer> triangleVB;
        triangleVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        triangleVB->SetLayout(layout);
        m_TriangleVertexArray->AddVertexBuffer(triangleVB);
        
        // Create a shape with the indices of the previously stored vertices.
        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> triangleIB;
        triangleIB.reset(IndexBuffer::Create(indices, 3));
        m_TriangleVertexArray->SetIndexBuffer(triangleIB);

        m_SquareVertexArray.reset(VertexArray::Create());
        
        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        float squareVertices[4 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.f, 0.f,
        };
        BufferLayout squareLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout(squareLayout);
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, 6));
        
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
        m_Shader.reset(new OpenGLShader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        // PKE_CORE_TRACE("EVENT: {}", e.ToString());

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(PKE_BIND_FUNCTION(OnWindowClosed));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            // De-increment, as we go backwards, starting at the top of the stack.
            (*--it)->OnEvent(e);
            if (e.IsHandled()) break;
        }
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {
            RenderCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
            RenderCommand::Clear();
            
            // m_Camera.SetPosition({sin((float)Time::time) * 0.5f, sin((float)Time::time * 0.8f) * 0.5f, 0}); // Move the camera left and right with a sine wave.
            m_Camera.SetRotation(sin((float)Time::time) * 0.5f); // Rotate the camera with a sine wave.
            
            Renderer::BeginScene(m_Camera); // Cameras lights and all others things.
            Renderer::SubmitGeometry(m_SquareVertexArray, m_Shader); // Submitting the geometry to be rendered, with the shader and all the other stuff.
            Renderer::EndScene();
            
            Renderer::BeginScene(m_Camera); // Cameras lights and all others things.
            Renderer::SubmitGeometry(m_TriangleVertexArray, m_Shader); // Submitting the geometry to be rendered, with the shader and all the other stuff.
            Renderer::EndScene();
            
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClosed(WindowCloseEvent& windowCloseEvent)
    {
        m_IsRunning = false;
        return true;
    }
}
