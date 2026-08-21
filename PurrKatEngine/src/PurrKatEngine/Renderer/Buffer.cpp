#include "pkepch.h"
#include "Buffer.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    VertexBuffer* VertexBuffer::Create(uint32_t size)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLVertexBuffer(size);
        )
    }

    VertexBuffer* VertexBuffer::Create(const float* vertices, uint32_t size)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLVertexBuffer(vertices, size);
        )
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLIndexBuffer(indices, count);;
        )
    }
}
