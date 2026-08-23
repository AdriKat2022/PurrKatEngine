#include "pkepch.h"
#include "RenderCommand.h"

#include "glad/glad.h"
#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace PurrKatEngine
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

    
    
    void RenderCommand::Init()
    {
        s_RendererAPI->Init();
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(glm::vec4 color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    void RenderCommand::Clear()
    {
        s_RendererAPI->Clear();
    }

    void RenderCommand::DrawIndexed(const VertexArray* vertexArray, uint32_t indexCount)
    {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

    void RenderCommand::EnableDepthTest()
    {
        s_RendererAPI->EnableDepthTest();
    }

    void RenderCommand::DisableDepthTest()
    {
        s_RendererAPI->DisableDepthTest();
    }

    // TEMP
    void RenderCommand::BlitFramebuffer(uint32_t framebuffer, uint32_t sourceWidth, uint32_t sourceHeight, uint32_t destinationWidth, uint32_t destinationHeight)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glBlitFramebuffer(
            0, 0,
            sourceWidth,
            sourceHeight,

            0, 0,
            destinationWidth,
            destinationHeight,

            GL_COLOR_BUFFER_BIT,
            GL_NEAREST
        );

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
}
