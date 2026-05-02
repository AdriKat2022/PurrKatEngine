#pragma once
#include "Keys.h"
#include "PurrKatEngine/Core.h"

namespace PurrKatEngine
{
    class PKE_API Input
    {
    public:
        Input() = default;
        virtual ~Input() = default;
        
        static bool IsKeyPressed(KeyCode keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
        static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl((KeyCode)keyCode); }
        static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        static double GetMouseX() { return s_Instance->GetMouseXImpl(); }
        static double GetMouseY() { return s_Instance->GetMouseYImpl(); }
        static glm::dvec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual double GetMouseXImpl() = 0;
        virtual double GetMouseYImpl() = 0;
        virtual glm::dvec2 GetMousePositionImpl() = 0;
        
    private:
        static Input* s_Instance;
    };
}
