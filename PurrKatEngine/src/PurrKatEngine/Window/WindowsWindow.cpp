#include "pkepch.h"
#include "WindowsWindow.h"

#include "GLFW/glfw3.h"
#include "PurrKatEngine/Logs/Log.h"

namespace PurrKatEngine
{
    static bool s_GLFWInitialized = false;
    
    CREATE_WINDOW_SETUP(WindowsWindow)

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::SetVSync(const bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        
        m_Data.VSync = enabled;
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        PKE_CORE_DEBUG("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            PKE_CORE_ASSERT(success, "Could not initialize GLFW")
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
}
