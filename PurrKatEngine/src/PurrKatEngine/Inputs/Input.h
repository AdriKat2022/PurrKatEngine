#pragma once
#include "Keys.h"
#include "PurrKatEngine/Core.h"

namespace PurrKatEngine
{
    class PKE_API Input
    {
    public:
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode);
        
        static double GetMouseX();
        static double GetMouseY();
        static glm::dvec2 GetMousePosition();
    };
}
