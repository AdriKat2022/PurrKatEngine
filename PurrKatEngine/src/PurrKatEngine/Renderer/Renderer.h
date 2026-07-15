#pragma once
#include "OrthographicCamera.h"
#include "RendererAPI.h"
#include "Shader.h"

namespace PurrKatEngine
{
    class Renderer
    {
    public:
        static void Init();
        static void OnWindowResize(uint32_t width, uint32_t height);
        
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void BeginScene(OrthographicCamera& camera);
        static void SubmitGeometry(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
        static void EndScene();

    private:
        // Following is temporary to allow the renderer to have states.
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        
        static SceneData* s_SceneData;
    };
}
