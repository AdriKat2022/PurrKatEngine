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
}
