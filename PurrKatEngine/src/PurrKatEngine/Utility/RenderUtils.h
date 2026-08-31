#pragma once
#include "PurrKatEngine/Renderer/FrameBuffer.h"

namespace PurrKatEngine
{
    class RenderUtils
    {
    public:
        static bool IsDepthFormat(FrameBufferTextureFormat format)
        {
            switch (format)
            {
                case FrameBufferTextureFormat::Depth24Stencil8:
                    return true;
            }
            
            return false;
        }
        
    };
}
