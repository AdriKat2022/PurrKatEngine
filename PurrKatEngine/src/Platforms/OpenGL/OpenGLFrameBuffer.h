#pragma once
#include "PurrKatEngine/Renderer/FrameBuffer.h"

namespace PurrKatEngine
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecifications& specs);
        ~OpenGLFrameBuffer() override;
        
        void Invalidate() override;
        void Bind() override;
        void Unbind() override;
        
        uint32_t GetColorAttachmentRendererID() const override;
        FrameBufferSpecifications& GetSpecifications() override;

    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
        FrameBufferSpecifications m_FrameBufferSpecifications;
    };
}
