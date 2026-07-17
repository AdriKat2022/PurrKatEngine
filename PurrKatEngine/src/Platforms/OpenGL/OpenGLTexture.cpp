#include "pkepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include "glad/glad.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    OpenGLTexture2D::OpenGLTexture2D(std::string path)
        : m_Path(std::move(path))
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
        PKE_CORE_ASSERT(data, "Failed to load image '{}'", m_Path);
        
        PKE_CORE_DEBUG("Creating ({}x{}) texture...", width, height);
        
        m_Width = width;
        m_Height = height;
        
        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            // Supports transparency
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            // No transparency channel
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        
        PKE_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
        
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    }
    
    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }
}
