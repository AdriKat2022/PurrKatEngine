#pragma once

#include "Core.h"

namespace MyGameEngine
{
    class MGE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
