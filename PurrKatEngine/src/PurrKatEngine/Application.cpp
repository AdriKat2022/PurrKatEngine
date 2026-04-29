#include "pkepch.h"

#include "Application.h"
#include "Logs/Log.h"
#include "Window/WindowsWindow.h"

namespace PurrKatEngine
{
    #define BIND_EVENT_FUNCTION(eventType, fn) [this](eventType& e) { return fn(e); }
    
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
        PKE_CORE_INFO("EVENT: {0}", e.ToString());
        
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(WindowCloseEvent, OnWindowClosed));
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
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}
