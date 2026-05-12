#include "pkepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace PurrKatEngine
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLVertexArray();
        }

        PKE_CORE_ASSERT(false, "Invalid RendererAPI.")
        return nullptr;
    }
}
