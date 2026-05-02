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

        // m_ImGuiLayer = new ImGuiLayer();
        // PushOverlay(m_ImGuiLayer);
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
            // De-increment, as we go backwards
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
            static float currentRed, currentGreen, currentBlue; 
            glClearColor(currentRed, currentGreen, currentBlue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

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
