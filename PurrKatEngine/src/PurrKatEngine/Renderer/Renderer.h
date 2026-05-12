#pragma once
#include "RendererAPI.h"

namespace PurrKatEngine
{
    class Renderer
    {
    public:
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void BeginScene();
        static void SubmitGeometry(const std::shared_ptr<VertexArray>& vertexArray);
        static void EndScene();
    };
}
