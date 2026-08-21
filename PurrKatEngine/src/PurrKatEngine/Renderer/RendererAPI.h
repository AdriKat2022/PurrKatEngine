#pragma once
#include "VertexArray.h"

#define SWITCH_ON_RENDERER_API(cases) switch (RendererAPI::GetAPI()) \
{ cases } \
\
PKE_CORE_ASSERT(false, "Invalid RendererAPI.") \
return nullptr;

#define API_CASE_NONE                     case RendererAPI::API::None:
#define API_CASE_NONE_NOT_SUPPORTED       case RendererAPI::API::None: PKE_CORE_ASSERT(false, "Having no RendererAPI is currently not supported.") return nullptr;

#define API_CASE_OPENGL                   case RendererAPI::API::OpenGL:
#define API_CASE_OPENGL_NOT_SUPPORTED     case RendererAPI::API::OpenGL: PKE_CORE_ASSERT(false, "Using OpenGL as a RendererAPI is currently not supported.") return nullptr;

namespace PurrKatEngine
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            OpenGL = 1,
            // DirectX = 2,
            // Vulkan = 3,
        };

    public:
        virtual ~RendererAPI() = default;
        
        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const VertexArray* vertexArray, uint32_t indexCount = 0) = 0;
        
        virtual void EnableDepthTest() = 0;
        virtual void DisableDepthTest() = 0;

        static API GetAPI() { return s_API; }
        
    private:
        static API s_API;
    };
}
