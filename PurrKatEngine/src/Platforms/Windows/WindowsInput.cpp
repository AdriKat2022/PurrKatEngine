#include "pkepch.h"

#include "WindowsWindow.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/Inputs/Input.h"

namespace PurrKatEngine
{
    bool Input::IsKeyPressed(KeyCode keyCode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, KeyCodeToGlfwCharCode(keyCode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseButtonCode mouseButtonCode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, (int)mouseButtonCode);
        return state == GLFW_PRESS;
    }

    glm::dvec2 Input::GetMousePosition()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }

    double Input::GetMouseX()
    {
        glm::dvec2 pos = GetMousePosition();
        return pos.x;
    }

    double Input::GetMouseY()
    {
        glm::dvec2 pos = GetMousePosition();
        return pos.y;
    }
}
