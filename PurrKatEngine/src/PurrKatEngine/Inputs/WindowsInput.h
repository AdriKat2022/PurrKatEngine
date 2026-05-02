#pragma once
#include "Input.h"

namespace PurrKatEngine
{
    class WindowsInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(KeyCode keyCode) override;
        bool IsMouseButtonPressedImpl(int button) override;
        double GetMouseXImpl() override;
        double GetMouseYImpl() override;
        glm::dvec2 GetMousePositionImpl() override;
    };
}
