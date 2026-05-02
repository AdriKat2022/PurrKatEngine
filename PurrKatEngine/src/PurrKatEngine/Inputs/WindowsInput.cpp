#include "pkepch.h"
#include "WindowsInput.h"

#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Window/WindowsWindow.h"

namespace PurrKatEngine
{
    Input* Input::s_Instance = new WindowsInput();
    
    bool WindowsInput::IsKeyPressedImpl(KeyCode keyCode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, KeyCodeToGlfwCharCode(keyCode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    Vector2d WindowsInput::GetMousePositionImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return Vector2d(xpos, ypos);
    }

    double WindowsInput::GetMouseXImpl()
    {
        auto[x,y] = GetMousePositionImpl();
        return x;
    }

    double WindowsInput::GetMouseYImpl()
    {
        auto[x,y] = GetMousePositionImpl();
        return y;
    }
}
