#include "pkepch.h"
#include "FrameBuffer.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLFrameBuffer.h"

namespace PurrKatEngine
{
    FrameBuffer* FrameBuffer::Create(const FrameBufferSpecifications& specs)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLFrameBuffer(specs);
        )
    }
    
    Ref<FrameBuffer> FrameBuffer::CreateRef(const FrameBufferSpecifications& specs)
    {
        return PurrKatEngine::CreateRef<FrameBuffer>(Create(specs));
    }
}
