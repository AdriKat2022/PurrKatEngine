#include "pkepch.h"

#include "Application.h"

#include "glad/glad.h"
#include "Logs/InternalLog.h"

namespace PurrKatEngine
{
    Application* Application::s_Instance = nullptr;
    
    Application::Application()
    {
        PKE_CORE_ASSERT(s_Instance == nullptr, "An application already exists.")
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(PKE_BIND_FUNCTION(OnEvent));
    }

    Application::~Application()
    {
    }
    
    void Application::OnEvent(Event& e)
    {
        InternalLog::GetCoreLogger()->info("EVENT: {}", e.ToString());
        
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(PKE_BIND_FUNCTION(OnWindowClosed));
        dispatcher.Dispatch<MouseMovedEvent>(PKE_BIND_FUNCTION(OnMouseMove));
        dispatcher.Dispatch<MouseScrollEvent>(PKE_BIND_FUNCTION(OnMouseScroll));
        
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

    float currentRed, currentGreen, currentBlue;
    
    bool Application::OnMouseMove(const MouseMovedEvent& cursorPosEvent)
    {
        currentRed = (float)cursorPosEvent.GetX() / (float)m_Window->GetWidth();
        currentGreen = (float)cursorPosEvent.GetY() / (float)m_Window->GetHeight();
        
        return true;
    }

    bool Application::OnMouseScroll(const MouseScrollEvent& scrollEvent)
    {
        currentBlue += scrollEvent.GetYOffset() / 10.f;
        currentBlue = MATHF_CLAMP_01(currentBlue);

        return true;
    }

    void Application::Run()
    {
        while (m_IsRunning)
        {
            glClearColor(currentRed, currentGreen, currentBlue, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            
            m_Window->OnUpdate();
        }
    }
}
