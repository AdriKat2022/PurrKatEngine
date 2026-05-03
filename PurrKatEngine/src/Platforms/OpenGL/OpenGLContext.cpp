#include "pkepch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        PKE_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        // Initial use of glad
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PKE_CORE_ASSERT(status, "Failed to initialize GLAD")
    }

    void OpenGLContext::SwapBuffers()
    {
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
        glfwSwapBuffers(m_WindowHandle);
    }
    
}
