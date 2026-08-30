#pragma once

namespace PurrKatEngine
{
    enum class ImageFilterType : unsigned char { Linear, Nearest };
    
    struct FrameBufferSpecifications
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;
        
        ImageFilterType UpscalingFilterType = ImageFilterType::Linear;
        
        bool SwapChainTarget = false;
    };
    
    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;
        
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual void Invalidate() = 0;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void ScaleFrom(const FrameBuffer& source) = 0;
        
        virtual uint32_t GetRendererID() const = 0;
        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual FrameBufferSpecifications& GetSpecifications() = 0;
        virtual const FrameBufferSpecifications& GetSpecifications() const = 0;

        static void PrintTextureInfo(uint32_t textureID);

        static FrameBuffer* Create(const FrameBufferSpecifications& specs);
        static Ref<FrameBuffer> CreateRef(const FrameBufferSpecifications& specs);
    };
}
