#include "pkepch.h"
#include "Input.h"

namespace PurrKatEngine
{
    float Input::GetAxis(KeyCode negativeKey, KeyCode positiveKey)
    {
        float axis = 0.0f;
        if (IsKeyPressed(positiveKey)) axis += 1.0f;
        if (IsKeyPressed(negativeKey)) axis -= 1.0f;
        return axis;
    }

    glm::vec2 Input::GetAxis2D(KeyCode upKey, KeyCode leftKey, KeyCode downKey, KeyCode rightKey)
    {
        float x = 0.0f, y = 0.0f;
        if (IsKeyPressed(rightKey)) x += 1.0f;
        if (IsKeyPressed(leftKey)) x -= 1.0f;
        if (IsKeyPressed(upKey)) y += 1.0f;
        if (IsKeyPressed(downKey)) y -= 1.0f;
        return {x, y};
    }
}
