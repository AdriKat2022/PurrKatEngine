#include "pkepch.h"

#include "Application.h"
#include "Events/WindowResizeEvent.h"
#include "Logs/Log.h"

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
        while (true)
        {
            
        }
    }
}
