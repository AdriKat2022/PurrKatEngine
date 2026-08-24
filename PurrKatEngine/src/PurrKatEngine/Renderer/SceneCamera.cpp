#include "pkepch.h"
#include "SceneCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

namespace PurrKatEngine
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjectionMatrix();
    }

    SceneCamera::SceneCamera(float size, float nearClip, float farClip)
        : m_OrthographicSize(size), m_OrthographicNear(nearClip), m_OrthographicFar(farClip)
    {
        RecalculateProjectionMatrix();
    }

    SceneCamera::AspectRatioAdjustmentMode SceneCamera::GetAspectRatioAdjustementMode() const
    {
        return m_AspectRatioAdjustementMode;
    }

    void SceneCamera::SetOrthographicSize(float size)
    {
        m_OrthographicSize = size;
        RecalculateProjectionMatrix();
    }
    
    float SceneCamera::GetOrthographicSize() const
    {
        return m_OrthographicSize;
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        
        RecalculateProjectionMatrix();
    }

    void SceneCamera::SetAspectRatioAdjustementMode(AspectRatioAdjustmentMode adjustementMode)
    {
        m_AspectRatioAdjustementMode = adjustementMode;
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        
        RecalculateProjectionMatrix();
    }

    void SceneCamera::RecalculateProjectionMatrix()
    {
        float orthoLeft, orthoBottom, orthoTop, orthoRight;
        float aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;

        if (m_AspectRatioAdjustementMode == AspectRatioAdjustmentMode::MatchWidth)
        {
            orthoLeft = -m_OrthographicSize * 0.5f;
            orthoRight = m_OrthographicSize * 0.5f;
            orthoBottom = -m_OrthographicSize / aspectRatio * 0.5f;
            orthoTop = m_OrthographicSize / aspectRatio * 0.5f;
        }
        else if (m_AspectRatioAdjustementMode == AspectRatioAdjustmentMode::MatchHeight)
        {
            orthoLeft = -m_OrthographicSize * aspectRatio * 0.5f;
            orthoRight = m_OrthographicSize * aspectRatio * 0.5f;
            orthoBottom = -m_OrthographicSize * 0.5f;
            orthoTop = m_OrthographicSize * 0.5f;
        }
        else
        {
            orthoLeft = -m_OrthographicSize * 0.5f;
            orthoRight = m_OrthographicSize * 0.5f;
            orthoBottom = -m_OrthographicSize * 0.5f;
            orthoTop = m_OrthographicSize * 0.5f;
        }

        m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
}
