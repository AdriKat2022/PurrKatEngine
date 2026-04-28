#include "pkepch.h"
#include "WindowsWindow.h"

#include "GLFW/glfw3.h"
#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Events/WindowCloseEvent.h"
#include "PurrKatEngine/Events/WindowResizeEvent.h"
#include "PurrKatEngine/Logs/Log.h"

#define GET_WINDOW_DATA_PTR(window) (WindowData*)glfwGetWindowUserPointer(window)

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

    void WindowsWindow::SetupGLFWCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);
            data->Height = height;
            data->Width = width;
            
            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);
            
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            switch (action)
            {
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, false);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    data->EventCallback(event);
                    break;
                }
                default: ;
            }
        });
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
        
        SetupGLFWCallbacks();
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
