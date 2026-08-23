#pragma once
#include "PurrKatEngine/Renderer/FrameBuffer.h"

namespace PurrKatEngine
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecifications& specs);
        ~OpenGLFrameBuffer() override;
        
        void Resize(uint32_t width, uint32_t height) override;
        void Invalidate() override;
        void Bind() override;
        void Unbind() override;

        void ScaleFrom(const FrameBuffer& source) override;
        
        uint32_t GetRendererID() const override;
        uint32_t GetColorAttachmentRendererID() const override;
        FrameBufferSpecifications& GetSpecifications() override;
        const FrameBufferSpecifications& GetSpecifications() const override;

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        FrameBufferSpecifications m_FrameBufferSpecifications;
    };
}
