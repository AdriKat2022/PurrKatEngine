#include "pkepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace PurrKatEngine
{
    // ************** TEXTURE 2D *******************

    Texture2D* Texture2D::Create(uint32_t width, uint32_t height, const TextureOptions& textureOptions)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLTexture2D(width, height, textureOptions);
        )
    }

    Texture2D* Texture2D::Create(const std::string& path, const TextureOptions& textureOptions)
    {
        SWITCH_ON_RENDERER_API(
            API_CASE_NONE_NOT_SUPPORTED
            API_CASE_OPENGL return new OpenGLTexture2D(path, textureOptions);
        )
    }

    Ref<Texture2D> Texture2D::CreateRef(uint32_t width, uint32_t height, const TextureOptions& textureOptions) { return PurrKatEngine::CreateRef<Texture2D>(Create(width, height, textureOptions)); }
    Ref<Texture2D> Texture2D::CreateRef(const std::string& path, const TextureOptions& textureOptions) { return PurrKatEngine::CreateRef<Texture2D>(Create(path, textureOptions)); }
}
