#pragma once
#include "Camera.h"

namespace PurrKatEngine
{
    class SceneCamera : public Camera
    {
    public:
        enum class AspectRatioAdjustmentMode : byte
        {
            FixedRatio,
            MatchWidth,
            MatchHeight,
        };
        
    public:
        SceneCamera();
        SceneCamera(float size, float nearClip, float farClip);
        
        void SetOrthographic(float size, float nearClip, float farClip);
        
        void SetOrthographicSize(float size);
        void SetAspectRatioAdjustementMode(AspectRatioAdjustmentMode adjustementMode);
        
        float GetOrthographicSize() const;
        AspectRatioAdjustmentMode GetAspectRatioAdjustementMode() const;
        
        void SetViewportSize(uint32_t width, uint32_t height);
        
        void RecalculateProjectionMatrix();

    private:
        float m_OrthographicSize = 10;
        float m_OrthographicNear = -1;
        float m_OrthographicFar = 1;
        
        AspectRatioAdjustmentMode m_AspectRatioAdjustementMode = AspectRatioAdjustmentMode::MatchHeight;
        
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;
        
        
    };
}
