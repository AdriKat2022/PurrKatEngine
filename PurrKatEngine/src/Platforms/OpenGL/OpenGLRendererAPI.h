#pragma once
#include "PurrKatEngine/Renderer/RendererAPI.h"

namespace PurrKatEngine
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        ~OpenGLRendererAPI();
        
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        
        void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
    
}
