#pragma once

#include "OrthographicCamera.h"
#include "PurrKatEngine/Components/Standard1DInputController.h"
#include "PurrKatEngine/Components/Standard2DInputController.h"
#include "PurrKatEngine/Events/Event.h"
#include "PurrKatEngine/Events/MouseScrollEvent.h"
#include "PurrKatEngine/Events/WindowResizeEvent.h"

namespace PurrKatEngine
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, float zoomLevel, bool useScrollToZoom = false);
        
        OrthographicCamera& GetCamera() { return m_Camera; }

        void OnUpdate();
        void OnEvent(Event& e);
        
        bool EnableMovement = true;
        bool EnableRotation = true;
        bool EnableZoom = true;
        
    private:
        bool OnMouseScroll(MouseScrollEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
        
    private:
        float m_AspectRatio = 1.0f;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;
        Standard2DInputController m_CameraMovementInputController;
        Standard1DInputController m_CameraRotationInputController;
    };
}
