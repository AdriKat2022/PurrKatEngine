#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"

namespace PurrKatEngine
{
    class RenderCommand
    {
    public:
        static void Init();
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        
        static void SetClearColor(glm::vec4 color);
        static void Clear();
        static void DrawIndexed(const VertexArray* vertexArray, uint32_t indexCount = 0);

    private:
        static RendererAPI* s_RendererAPI;
    };
    
}
