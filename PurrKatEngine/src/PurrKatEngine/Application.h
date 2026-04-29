#pragma once

#include "Core.h"
#include "Events/WindowCloseEvent.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    class PKE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e); // Will be run each time an event is triggered by the window.
        
    private:
        bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);
        
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
