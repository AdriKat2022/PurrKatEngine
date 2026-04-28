#include "pkepch.h"

#include "Application.h"
#include "Logs/Log.h"
#include "Events/WindowResizeEvent.h"
#include "Window/WindowsWindow.h"

namespace PurrKatEngine
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
    }

    Application::~Application()
    {
    }
    
    void Application::OnEvent(Event& e)
    {
        PKE_CORE_INFO("EVENT: {0}", e.ToString());

        // TODO: Dispatch the event.
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
