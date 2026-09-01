#include "pkepch.h"
#include "OpenGLFrameBuffer.h"

#include "PurrKatEngine/Logs/InternalLog.h"
#include "glad/glad.h"
#include "PurrKatEngine/Utility/RenderUtils.h"

namespace PurrKatEngine
{
    // *** Debugging and error checking.
    void FrameBuffer::PrintTextureInfo(uint32_t textureID)
    {
        GLint minFilter, magFilter;

        glGetTextureParameteriv(textureID, GL_TEXTURE_MIN_FILTER, &minFilter);
        glGetTextureParameteriv(textureID, GL_TEXTURE_MAG_FILTER, &magFilter);

        PKE_CORE_TRACE("FB texture {}: min={}, mag={}", textureID, minFilter, magFilter);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        PKE_CORE_TRACE("OpenGL viewport: {}x{}", viewport[2], viewport[3]);
    }
    
    static constexpr uint32_t s_MaxFrameBufferSize = 8192;

    namespace Utils
    {
        static GLenum TextureTarget(bool isMultiSample)
        {
            return isMultiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }
        
        static GLenum ColorTextureFormat(FrameBufferTextureFormat textureFormat)
        {
            switch (textureFormat)
            {
                case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
                case FrameBufferTextureFormat::RED_INTEGER: return GL_R32I;
                    
                default:
                    PKE_CORE_ERROR("Unknown FrameBufferTextureFormat for ColorTexture: {}", (int)textureFormat);
                    return GL_RGBA8;
            }
        }
        
        static GLenum DepthTextureFormat(FrameBufferTextureFormat textureFormat)
        {
            switch (textureFormat)
            {
                case FrameBufferTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
                    
                default:
                    PKE_CORE_ERROR("Invalid FrameBufferTextureFormat for DepthTexture: {}", (int)textureFormat);
                    return GL_DEPTH24_STENCIL8;
            }
        }
        
        static int GLFilterType(ImageFilterType filterType)
        {
            switch (filterType)
            {
                case ImageFilterType::Linear: return GL_LINEAR;
                case ImageFilterType::Nearest: return GL_NEAREST;
                    
                default:
                    PKE_CORE_ERROR("Unknown ImageFilterType: {}", (int)filterType);
                    return GL_NEAREST;
            }
        }
        
        static void CreateTextures(uint32_t* textureIds, size_t count, bool isMultiSample)
        {
            glCreateTextures(TextureTarget(isMultiSample), (GLsizei)count, textureIds);
        }

        static void BindTexture(uint32_t textureId, bool multiSample)
        {
            glBindTexture(TextureTarget(multiSample), textureId);
        }
        
        static void AttachColorTexture(uint32_t rendererId, uint32_t textureId, int samples, const FrameBufferTextureSpecifications& specs, GLsizei width, GLsizei height, int index)
        {
            GLenum internalFormat = ColorTextureFormat(specs.TextureFormat);
            
            bool multiSample = samples > 1;
            if (multiSample)
            {
                glTextureStorage2DMultisample(textureId, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTextureStorage2D(textureId, 1, internalFormat, width, height);
                
                auto filter = GLFilterType(specs.FilterType);
                
                // Set texture parameters for filtering and wrapping
                glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, filter);
                glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, filter);
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // To parameterise
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // To parameterise
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // To parameterise
            }
            
            glNamedFramebufferTexture(rendererId, GL_COLOR_ATTACHMENT0 + index, textureId, 0);
        }
        
        static void AttachDepthTexture(uint32_t rendererId, uint32_t textureId, int samples, const FrameBufferTextureSpecifications& specs, GLenum attachmentType, GLsizei width, GLsizei height)
        {
            GLenum format = DepthTextureFormat(specs.TextureFormat);
            
            bool multiSample = samples > 1;
            if (multiSample)
            {
                glTextureStorage2DMultisample(textureId, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTextureStorage2D(textureId, 1, format, width, height);
                
                auto filter = GLFilterType(specs.FilterType);
                
                // Set texture parameters for filtering and wrapping
                glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, filter);
                glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, filter);
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // To parameterise
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // To parameterise
                glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // To parameterise
            }
            
            glNamedFramebufferTexture(rendererId, attachmentType, textureId, 0);
        }
        
    }
    
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& specs)
        : m_FrameBufferSpecifications(specs)
    {
        // Sort out the attachements specifications here (to make sure the depth attachment identified).
        for (const auto& attachmentSpec : specs.AttachmentsSpecs.Attachments)
        {
            if (RenderUtils::IsDepthFormat(attachmentSpec.TextureFormat))
                m_DepthAttachmentSpec = attachmentSpec;
            else
                m_ColorAttachmentSpecs.emplace_back(attachmentSpec);
        }
        
        OpenGLFrameBuffer::Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_DepthAttachment);
        if (!m_ColorAttachments.empty())
            glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
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

    void OpenGLFrameBuffer::EraseData()
    {
        if (m_RendererID)
        {
            // Delete previous state
            glDeleteFramebuffers(1, &m_RendererID); m_RendererID = 0;
            glDeleteTextures(1, &m_DepthAttachment); m_DepthAttachment = 0;
            if (!m_ColorAttachments.empty())
                glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
            m_ColorAttachments.clear();
        }
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        // Recreate the whole state.
        EraseData();
        
        int samples = (int)m_FrameBufferSpecifications.Samples;
        bool multiSample = samples > 1;
        GLsizei frameBufferWidth = (GLsizei)m_FrameBufferSpecifications.Width;
        GLsizei frameBufferHeight = (GLsizei)m_FrameBufferSpecifications.Height;
        
        glCreateFramebuffers(1, &m_RendererID);
        
        // ---------- Attachements ----------
        // An attachement is either a color buffer or a depth buffer.
        // A framebuffer can have multiple color attachments, but only one depth/stencil attachment.
        
        // Color attachments
        if (!m_ColorAttachmentSpecs.empty())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
            Utils::CreateTextures(m_ColorAttachments.data(), m_ColorAttachments.size(), multiSample);
            for (size_t i = 0; i < m_ColorAttachmentSpecs.size(); i++)
            {
                Utils::BindTexture(m_ColorAttachments[i], multiSample);
                Utils::AttachColorTexture(m_RendererID, m_ColorAttachments[i], samples, m_ColorAttachmentSpecs[i], frameBufferWidth, frameBufferHeight, i);
            }
        }
        
        // Depth attachment
        if (m_DepthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None)
        {
            Utils::CreateTextures(&m_DepthAttachment, 1, multiSample);
            Utils::BindTexture(m_DepthAttachment, multiSample);
            Utils::AttachDepthTexture(m_RendererID, m_DepthAttachment, samples, m_DepthAttachmentSpec, GL_DEPTH_STENCIL_ATTACHMENT, frameBufferWidth, frameBufferHeight);
        }
        
        // Optimize the framebuffer for rendering by specifying which color attachments to draw to.
        if (m_ColorAttachments.size() > 1)
        {
            // Set the draw buffers for the framebuffer to use all color attachments.
            PKE_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Too many color attachments. Maximum is 4.")
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glNamedFramebufferDrawBuffers(m_RendererID, (GLsizei)m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth-pass, disable drawing.
            glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
        }
        
        // -------- Debugging and error checking --------
        CheckFrameBufferIntegrity();
    }

    void OpenGLFrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, (GLsizei)m_FrameBufferSpecifications.Width, (GLsizei)m_FrameBufferSpecifications.Height);
    }

    void OpenGLFrameBuffer::Unbind() const
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
            GL_COLOR_BUFFER_BIT, /*m_FrameBufferSpecifications.UpscalingFilterType == ImageFilterType::Linear ? GL_LINEAR : */GL_NEAREST
        );

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFrameBuffer::GetRendererID() const { return m_RendererID; }

    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(uint32_t index) const { return m_ColorAttachments[index]; }

    FrameBufferSpecifications& OpenGLFrameBuffer::GetSpecifications() { return m_FrameBufferSpecifications; }

    const FrameBufferSpecifications& OpenGLFrameBuffer::GetSpecifications() const { return m_FrameBufferSpecifications; }
    
    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) const
    {
        PKE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds.");
     
        // Dodgy way to read pixel data from a framebuffer. This assumes the framebuffer is bound and the correct attachment is selected.
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFrameBuffer::CheckFrameBufferIntegrity()
    {
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
        if (status == GL_FRAMEBUFFER_COMPLETE)
            return;
        
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
