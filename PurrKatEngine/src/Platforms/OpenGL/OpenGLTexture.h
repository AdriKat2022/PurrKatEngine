#pragma once
#include "glad/glad.h"
#include "PurrKatEngine/Renderer/Texture.h"

namespace PurrKatEngine
{
    // class OpenGLTexture : public Texture
    // {
    //
    // };
    
    class OpenGLTexture2D : public Texture2D
    {
    public:
        explicit OpenGLTexture2D(uint32_t width, uint32_t height);
        explicit OpenGLTexture2D(std::string path);
        
        ~OpenGLTexture2D() override;
        
        void SetData(void* data, uint32_t size) override;

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        
        void Bind(uint32_t slot = 0) const override;
        
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        
        GLenum m_InternalFormat, m_DataFormat;
    };
}
