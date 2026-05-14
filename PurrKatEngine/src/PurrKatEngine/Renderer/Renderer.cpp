#include "pkepch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace PurrKatEngine
{
    Renderer::SceneData* Renderer::s_SceneData = new SceneData;
    
    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::SubmitGeometry(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
}
