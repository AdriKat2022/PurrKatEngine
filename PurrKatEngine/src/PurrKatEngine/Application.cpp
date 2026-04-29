#include "pkepch.h"

#include "Application.h"
#include "Logs/Log.h"
#include "Utility/Mathf.h"
#include "Window/WindowsWindow.h"

namespace PurrKatEngine
{
    #define BIND_EVENT_FUNCTION(eventType, fn) [this](eventType& e) { return fn(e); }  // NOLINT(bugprone-macro-parentheses)
    
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Event, OnEvent));
    }

    Application::~Application()
    {
    }
    
    void Application::OnEvent(Event& e)
    {
        PKE_CORE_INFO("EVENT: {}", e.ToString());
        
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(WindowCloseEvent, OnWindowClosed));
        dispatcher.Dispatch<CursorPosEvent>(BIND_EVENT_FUNCTION(CursorPosEvent, OnMouseMove));
        dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FUNCTION(MouseScrollEvent, OnMouseScroll));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            // De-increment, as we go backwards
            (*--it)->OnEvent(e);
            if (e.IsHandled()) break;
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& windowCloseEvent)
    {
        m_IsRunning = false;
        return true;
    }

    float currentRed, currentGreen, currentBlue;
    
    bool Application::OnMouseMove(const CursorPosEvent& cursorPosEvent)
    {
        currentRed = cursorPosEvent.GetX() / m_Window->GetWidth();
        currentGreen = cursorPosEvent.GetY() / m_Window->GetHeight();
        
        return true;
    }

    bool Application::OnMouseScroll(const MouseScrollEvent& scrollEvent)
    {
        currentBlue += scrollEvent.GetYOffset() / 10.f;
        currentBlue = MATHF_CLAMP_01(currentBlue);
        
        PKE_CORE_DEBUG("BLUE: {}", currentBlue);

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
