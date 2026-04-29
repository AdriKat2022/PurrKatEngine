#pragma once

#include "Core.h"
#include "Events/CursorPosEvent.h"
#include "Events/MouseScrollEvent.h"
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
        
    protected:
        virtual bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);
        virtual bool OnMouseMove(const CursorPosEvent& cursorPosEvent);
        virtual bool OnMouseScroll(const MouseScrollEvent& scrollEvent);

    private:
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
