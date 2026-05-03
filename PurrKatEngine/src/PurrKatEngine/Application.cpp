#include "pkepch.h"

#include "Application.h"

#include "imgui.h"
#include "glad/glad.h"
#include "Inputs/Input.h"
#include "Logs/InternalLog.h"
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

        // Bind the storage of the vertex array buffer and vertex buffer.
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);
        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        // Create points (vertices) and store them into the vertex array buffer.
        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        // Create a shape with the indices of the previously stored vertices.
        unsigned int indices[3] = { 0, 1, 2 };

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    Application::~Application()
    {
    }
    
    void Application::OnEvent(Event& e)
    {
        // PKE_CORE_TRACE("EVENT: {}", e.ToString());
        
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(PKE_BIND_FUNCTION(OnWindowClosed));
        
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            // De-increment, as we go backwards, starting at the top of the stack.
            (*--it)->OnEvent(e);
            if (e.IsHandled()) break;
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

    void Application::Run()
    {
        while (m_IsRunning)
        {
            // static float currentRed, currentGreen, currentBlue; 
            // glClearColor(currentRed, currentGreen, currentBlue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
            
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            
            m_Window->OnUpdate();
        }
    }
}
