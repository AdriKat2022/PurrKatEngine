#include "pkepch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace PurrKatEngine
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::SubmitGeometry(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
}
