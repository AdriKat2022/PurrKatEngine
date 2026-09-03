#pragma once
#include "Keys.h"
#include "PurrKatEngine/Core.h"

namespace PurrKatEngine
{
    class PKE_API Input
    {
    public:
        static bool IsKeyPressedOrNone(KeyCode keyCode) { return keyCode == KeyCode::None || IsKeyPressed(keyCode); }
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsMouseButtonPressed(MouseButtonCode mouseButtonCode);
        
        static double GetMouseX();
        static double GetMouseY();
        static glm::dvec2 GetMousePosition();
        
        // Utility (premade)
        static float GetAxis(KeyCode negativeKey, KeyCode positiveKey);
        static glm::vec2 GetAxis2D(KeyCode upKey, KeyCode leftKey, KeyCode downKey, KeyCode rightKey);
    };
}
