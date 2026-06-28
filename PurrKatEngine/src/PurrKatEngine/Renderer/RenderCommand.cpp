#include "pkepch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace PurrKatEngine
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
    
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
