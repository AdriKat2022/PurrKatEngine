#pragma once
#include "PurrKatEngine/Inputs/Input.h"

namespace PurrKatEngine
{
    class WindowsInput : public Input
    {
    protected:
        bool IsKeyPressedImpl(KeyCode keyCode) override;
        bool IsMouseButtonPressedImpl(MouseButtonCode mouseButtonCode) override;
        double GetMouseXImpl() override;
        double GetMouseYImpl() override;
        glm::dvec2 GetMousePositionImpl() override;
    };
}
