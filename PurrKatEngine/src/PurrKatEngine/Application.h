#pragma once

#include "Core.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    class PKE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
