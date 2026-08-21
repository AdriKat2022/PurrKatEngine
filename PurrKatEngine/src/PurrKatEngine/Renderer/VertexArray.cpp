#include "pkepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace PurrKatEngine
{
    VertexArray* VertexArray::Create()
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLVertexArray();
        )
    }
}
