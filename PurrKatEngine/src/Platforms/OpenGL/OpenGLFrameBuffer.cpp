#include "pkepch.h"
#include "OpenGLFrameBuffer.h"

#include "PurrKatEngine/Logs/InternalLog.h"
#include "glad/glad.h"

namespace PurrKatEngine
{
    static constexpr uint32_t s_MaxFrameBufferSize = 8192; 
    
    void FrameBuffer::PrintTextureInfo(uint32_t textureID)
    {
        GLint minFilter;
        GLint magFilter;

        glGetTextureParameteriv(
            textureID,
            GL_TEXTURE_MIN_FILTER,
            &minFilter
        );

        glGetTextureParameteriv(
            textureID,
            GL_TEXTURE_MAG_FILTER,
            &magFilter
        );

        PKE_CORE_TRACE(
            "FB texture {}: min={}, mag={}",
            textureID,
            minFilter,
            magFilter
        );
        
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        PKE_CORE_TRACE(
            "OpenGL viewport: {}x{}",
            viewport[2],
            viewport[3]
        );
    }
    
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& specs)
        : m_FrameBufferSpecifications(specs)
    {
        OpenGLFrameBuffer::Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
        {
            PKE_CORE_WARN("Attempted to resize framebuffer to {0}, {1} which is an invalid size", width, height);
            return;
        }
        
        if (width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
        {
            PKE_CORE_WARN("Attempted to resize framebuffer to {0}, {1} which exceeds the maximum size of {2}, {3}", width, height, s_MaxFrameBufferSize, s_MaxFrameBufferSize);
            return;
        }
        
        m_FrameBufferSpecifications.Width = width;
        m_FrameBufferSpecifications.Height = height;
        
        Invalidate();
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        // Recreate the whole state.
        if (m_RendererID)
        {
            // Delete previous state
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
            
            m_RendererID = 0;
            m_ColorAttachment = 0;
            m_DepthAttachment = 0;
        }
        
        glCreateFramebuffers(1, &m_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);

        glTextureStorage2D(
            m_ColorAttachment,
            1,
            GL_RGBA8,
            (GLsizei)m_FrameBufferSpecifications.Width,
            (GLsizei)m_FrameBufferSpecifications.Height
        );

        auto filter = m_FrameBufferSpecifications.UpscalingFilterType == FilterType::Linear ? GL_LINEAR : GL_NEAREST;
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, filter);

        glNamedFramebufferTexture(
            m_RendererID,
            GL_COLOR_ATTACHMENT0,
            m_ColorAttachment,
            0
        );

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);

        glTextureStorage2D(
            m_DepthAttachment,
            1,
            GL_DEPTH24_STENCIL8,
            (GLsizei)m_FrameBufferSpecifications.Width,
            (GLsizei)m_FrameBufferSpecifications.Height
        );

        glNamedFramebufferTexture(
            m_RendererID,
            GL_DEPTH_STENCIL_ATTACHMENT,
            m_DepthAttachment,
            0
        );
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            switch (status)
            {
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    PKE_CORE_ERROR("INCOMPLETE_ATTACHMENT");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    PKE_CORE_ERROR("INCOMPLETE_MISSING_ATTACHMENT");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    PKE_CORE_ERROR("INCOMPLETE_DRAW_BUFFER");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    PKE_CORE_ERROR("INCOMPLETE_READ_BUFFER");
                    break;

                case GL_FRAMEBUFFER_UNSUPPORTED:
                    PKE_CORE_ERROR("UNSUPPORTED");
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    PKE_CORE_ERROR("INCOMPLETE_MULTISAMPLE");
                    break;

                default:
                    PKE_CORE_ERROR("Unknown framebuffer error: {}", status);
                    break;
            }
        }
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, (GLsizei)m_FrameBufferSpecifications.Width, (GLsizei)m_FrameBufferSpecifications.Height);
    }
    
    void OpenGLFrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::ScaleFrom(const FrameBuffer& source)
    {
        auto& src = source.GetSpecifications();

        const GLsizei srcWidth  = (GLsizei)src.Width;
        const GLsizei srcHeight = (GLsizei)src.Height;

        const GLsizei dstWidth = (GLsizei)m_FrameBufferSpecifications.Width;
        const GLsizei dstHeight = (GLsizei)m_FrameBufferSpecifications.Height;

        glBindFramebuffer(GL_READ_FRAMEBUFFER,(GLuint)source.GetRendererID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_RendererID);

        glBlitFramebuffer(
            0, 0, srcWidth, srcHeight,
            0, 0, dstWidth, dstHeight,
            GL_COLOR_BUFFER_BIT, m_FrameBufferSpecifications.UpscalingFilterType == FilterType::Linear ? GL_LINEAR : GL_NEAREST
        );

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFrameBuffer::GetRendererID() const { return m_RendererID; }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID() const { return m_ColorAttachment; }

    FrameBufferSpecifications& OpenGLFrameBuffer::GetSpecifications() { return m_FrameBufferSpecifications; }
    const FrameBufferSpecifications& OpenGLFrameBuffer::GetSpecifications() const { return m_FrameBufferSpecifications; }
}
