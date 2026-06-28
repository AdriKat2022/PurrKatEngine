#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"

namespace PurrKatEngine
{
    class RenderCommand
    {
    public:
        static void Init();
        static void SetClearColor(glm::vec4 color);
        static void Clear();
        static void DrawIndexed(const Ref<VertexArray>& vertexArray);

    private:
        static RendererAPI* s_RendererAPI;
    };
    
}
