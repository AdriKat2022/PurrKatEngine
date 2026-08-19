#include "pkepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace PurrKatEngine
{
    // ************** TEXTURE 2D *******************

    Texture2D* Texture2D::Create(uint32_t width, uint32_t height, const TextureOptions& textureOptions)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLTexture2D(width, height, textureOptions);
        }
        
        return nullptr;
    }

    Texture2D* Texture2D::Create(const std::string& path, const TextureOptions& textureOptions)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return new OpenGLTexture2D(path, textureOptions);
        }
        
        return nullptr;
    }

    Ref<Texture2D> Texture2D::CreateRef(uint32_t width, uint32_t height, const TextureOptions& textureOptions) { return PurrKatEngine::CreateRef<Texture2D>(Create(width, height, textureOptions)); }
    Ref<Texture2D> Texture2D::CreateRef(const std::string& path, const TextureOptions& textureOptions) { return PurrKatEngine::CreateRef<Texture2D>(Create(path, textureOptions)); }
}
