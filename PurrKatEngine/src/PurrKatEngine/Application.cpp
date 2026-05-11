#include "pkepch.h"

#include "Application.h"

#include "glad/glad.h"
#include "Inputs/Input.h"
#include "Logs/InternalLog.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "Renderer/BufferLayout.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        PKE_CORE_ASSERT(s_Instance == nullptr, "An application already exists.")
        s_Instance = this;
        
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(PKE_BIND_FUNCTION(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());
        
        // Create points (vertices) and store them into the vertex array buffer.
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.f, 0.f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
        };
        
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        
        
        // Create a shape with the indices of the previously stored vertices.
        uint32_t indices[3] = {0, 1, 2};

        m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        
        // Test shader code.
        std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
out vec3 v_Position;
out vec4 v_Color;
void main()
{
    v_Color = a_Color;
    v_Position = a_Position;
    gl_Position = vec4(a_Position*2, 1.0);
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
            static float currentRed, currentGreen, currentBlue = 0.1f; 
            glClearColor(currentRed, currentGreen, currentBlue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
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
