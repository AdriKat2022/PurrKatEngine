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
        void Bind() const override;
        void Unbind() const override;

        void EraseData();
        void ScaleFrom(const FrameBuffer& source) override;
        
        uint32_t GetRendererID() const override;
        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
        FrameBufferSpecifications& GetSpecifications() override;
        const FrameBufferSpecifications& GetSpecifications() const override;

        int ReadPixel(uint32_t attachmentIndex, int x, int y) const override;

    private:
        static void CheckFrameBufferIntegrity();

    private:
        uint32_t m_RendererID = 0; // Main object ID for the framebuffer
        FrameBufferSpecifications m_FrameBufferSpecifications;
        
        // Color attachments and their specifications
        std::vector<uint32_t> m_ColorAttachments; 
        std::vector<FrameBufferTextureSpecifications> m_ColorAttachmentSpecs;
        
        // Depth attachment and its specification
        uint32_t m_DepthAttachment = 0;
        FrameBufferTextureSpecifications m_DepthAttachmentSpec = {FrameBufferTextureFormat::None};
    };
}
