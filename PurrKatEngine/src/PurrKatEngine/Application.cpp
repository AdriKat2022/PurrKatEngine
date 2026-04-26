#include "pkepch.h"

#include "Application.h"
#include "Logs/Log.h"
#include "Events/WindowResizeEvent.h"
#include "Window/WindowsWindow.h"

namespace PurrKatEngine
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent windowEvent(1920, 1080);
        PKE_CORE_TRACE(windowEvent);

        WindowProps props{};
        WindowsWindow window{ WindowProps() };
        PKE_CORE_TRACE(window.ToString());
        
        while (true)
        {
            
        }
    }
}
