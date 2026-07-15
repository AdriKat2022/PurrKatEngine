#include "pkepch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace PurrKatEngine
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

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

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        s_RendererAPI->DrawIndexed(vertexArray);
    }
}
