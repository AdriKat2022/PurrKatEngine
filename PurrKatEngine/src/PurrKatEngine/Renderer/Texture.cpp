#include "pkepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace PurrKatEngine
{
    // ************** TEXTURE 2D *******************

    Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLTexture2D(width, height);
        }
        
        return nullptr;
    }

    Texture2D* Texture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLTexture2D(path);
        }
        
        return nullptr;
    }
}
