#include "pkepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace PurrKatEngine
{
    // ************** TEXTURE 2D *******************
    
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:     PKE_CORE_ASSERT(false, "Having No RendererAPI is currently not supported.") return nullptr;
            case RendererAPI::API::OpenGL:   return MakeRef<OpenGLTexture2D>(path);
        }
        
        return nullptr;
    }
}
