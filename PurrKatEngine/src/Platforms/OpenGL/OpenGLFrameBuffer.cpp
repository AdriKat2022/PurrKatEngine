#include "pkepch.h"
#include "OpenGLFrameBuffer.h"

#include "PurrKatEngine/Logs/InternalLog.h"
#include "glad/glad.h"

namespace PurrKatEngine
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& specs)
        : m_FrameBufferSpecifications(specs)
    {
        OpenGLFrameBuffer::Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
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

        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    }
    
    void OpenGLFrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID() const { return m_ColorAttachment; }

    FrameBufferSpecifications& OpenGLFrameBuffer::GetSpecifications() { return m_FrameBufferSpecifications; }
}
