#include "pkepch.h"
#include "WindowsWindow.h"
#include "PurrKatEngine/Window/Window.h"

#include "Platforms/OpenGL/OpenGLContext.h" // Can be changed for a different context (Vulkan for example).
#include "PurrKatEngine/Events/ApplicationEvents.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Renderer/GraphicsContext.h"

#define GET_WINDOW_DATA_PTR(window) (WindowData*)glfwGetWindowUserPointer(window)

namespace PurrKatEngine
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int errorCode, const char* description)
    {
        PKE_CORE_ERROR("GLFW Error ({}) {}", errorCode, description);
    }
    
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
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        
        m_Context = new OpenGLContext(m_Window); // Can swap OpenGL context for another context (e.g. Vulkan) by changing this line and the include.
        m_Context->Init();
        
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
        
        SetupGLFWCallbacks();
    }
    
    void WindowsWindow::SetupGLFWCallbacks() const
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

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keyCode, int scancode, int action, int mods)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            switch (action)
            {
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(keyCode);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(keyCode, false);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(keyCode, true);
                    data->EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            KeyTypedEvent event((int)keyCode);
            data->EventCallback(event);
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent pressedEvent(button);
                    data->EventCallback(pressedEvent);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent releasedEvent(button);
                    data->EventCallback(releasedEvent);
                    break;
                }
                default: ;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);
            
            MouseScrollEvent event((float)xOffset, (float)yOffset);
            data->EventCallback(event);
        });

        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            if (entered)
            {
                CursorEnterEvent event;
                data->EventCallback(event);
            }
            else
            {
                CursorExitEvent event;
                data->EventCallback(event);
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData* data = GET_WINDOW_DATA_PTR(window);

            MouseMovedEvent event(xPos, yPos);
            data->EventCallback(event);
        });
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
}
