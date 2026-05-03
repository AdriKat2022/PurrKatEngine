#pragma once
#include "PurrKatEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace PurrKatEngine
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);
        
        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}
