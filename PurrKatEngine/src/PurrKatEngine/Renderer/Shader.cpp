#include "pkepch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace PurrKatEngine
{
    Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None:
                PKE_CORE_ASSERT(false, "RendererAPI::None is currently not supported.")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSource, fragmentSrc);
        }
        
        return nullptr;
    }
}
