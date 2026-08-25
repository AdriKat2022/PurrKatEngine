#pragma once
#include "Camera.h"

namespace PurrKatEngine
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType : unsigned char
        {
            Perspective,
            Orthographic
        };
        
        enum class AspectRatioAdjustmentMode : unsigned char
        {
            Variable,
            MatchWidth,
            MatchHeight,
        };
        
    public:
        SceneCamera();
        SceneCamera(float size, float nearClip, float farClip);
        
        void SetPerspective(float pov, float nearClip, float farClip);
        void SetOrthographic(float size, float nearClip, float farClip);

        void SetProjectionType(ProjectionType projectionType) { m_ProjectionType = projectionType; RecalculateProjectionMatrix(); }
        void SetAspectRatioAdjustementMode(AspectRatioAdjustmentMode adjustementMode) { m_AspectRatioAdjustementMode = adjustementMode; RecalculateProjectionMatrix(); }

        ProjectionType GetProjectionType() const;
        AspectRatioAdjustmentMode GetAspectRatioAdjustementMode() const;

        void SetPerspectivePov(float pov) { m_PerspectivePov = pov; RecalculateProjectionMatrix(); }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjectionMatrix(); }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjectionMatrix(); }
        float GetPerspectivePov() const { return m_PerspectivePov; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix(); }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjectionMatrix(); }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjectionMatrix(); }
        float GetOrthographicSize() const { return m_OrthographicSize; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }

        void SetViewportSize(uint32_t width, uint32_t height);
        
        void RecalculateProjectionMatrix();

    private:
        float m_PerspectivePov = glm::radians(45.0f);
        float m_PerspectiveNear = 0.010f;
        float m_PerspectiveFar = 1000;
        
        float m_OrthographicSize = 10;
        float m_OrthographicNear = -1;
        float m_OrthographicFar = 1;
        
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        AspectRatioAdjustmentMode m_AspectRatioAdjustementMode = AspectRatioAdjustmentMode::MatchHeight;
        
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;
    };
}
