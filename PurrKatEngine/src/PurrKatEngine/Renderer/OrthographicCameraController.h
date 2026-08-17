#pragma once

#include "OrthographicCamera.h"
#include "PurrKatEngine/Components/Standard1DInputController.h"
#include "PurrKatEngine/Components/Standard2DInputController.h"
#include "PurrKatEngine/Events/Event.h"
#include "PurrKatEngine/Events/MouseScrollEvent.h"
#include "PurrKatEngine/Events/WindowResizeEvent.h"

namespace PurrKatEngine
{
    enum class AspectRatioAdjustmentMode : byte
    {
        None,
        MatchWidth,
        MatchHeight,
    };
    
    struct OrthographicCameraBounds
    {
        float Left, Right;
        float Top, Bottom;
        
        float GetWidth() const { return Right - Left; }
        float GetHeight() const { return Top - Bottom; }
    };
    
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, float zoomLevel, bool useScrollToZoom = false);
        
        OrthographicCamera& GetCamera() { return m_Camera; }

        void OnUpdate();
        void OnEvent(Event& e);
        
        void SetZoomLevel(float level) { m_ZoomLevel = level; }
        
        float GetZoomLevel() const { return m_ZoomLevel; }
        const OrthographicCameraBounds& GetCameraBounds() const { return m_CameraBounds; }
        
        bool EnableMovement = true;
        bool EnableRotation = true;
        bool EnableZoom = true;
        AspectRatioAdjustmentMode AspectRatioAdjustment = AspectRatioAdjustmentMode::MatchHeight;
        
    private:
        bool OnMouseScroll(MouseScrollEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        
    private:
        float m_AspectRatio = 1.0f;
        float m_ZoomLevel = 1.0f;
        float m_CameraRotation = 0;
        OrthographicCameraBounds m_CameraBounds;
        OrthographicCamera m_Camera;
        
        Standard2DInputController m_CameraMovementInputController;
        Standard1DInputController m_CameraRotationInputController;
    };
}
