#include "pkepch.h"
#include "WindowsInput.h"

#include "WindowsWindow.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Inputs/Input.h"

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

    glm::dvec2 WindowsInput::GetMousePositionImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }

    double WindowsInput::GetMouseXImpl()
    {
        glm::dvec2 pos = GetMousePositionImpl();
        return pos.x;
    }

    double WindowsInput::GetMouseYImpl()
    {
        glm::dvec2 pos = GetMousePositionImpl();
        return pos.y;
    }
}
