#include "pkepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
        }

        PKE_CORE_ASSERT(false, "Invalid RendererAPI.")
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(indices, count);
        }
        
        PKE_CORE_ASSERT(false, "Invalid RendererAPI.")
        return nullptr;
    }
}
