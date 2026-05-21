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

    void Renderer::SubmitGeometry(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix); // If using a single shader + static camera, this can be done once at the start, and never again.
        shader->UploadUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
}
