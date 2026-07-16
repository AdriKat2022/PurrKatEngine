#include "pkepch.h"
#include "OrthographicCameraController.h"

#include "PurrKatEngine/Inputs/Time.h"

namespace PurrKatEngine
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool useScrollToZoom)
        : EnableZoom(useScrollToZoom),
          m_AspectRatio(aspectRatio),
          m_ZoomLevel(zoomLevel),
          m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
          m_CameraMovementInputController([this](glm::vec2 input)
          {
              auto camPos = m_Camera.GetPosition();
              camPos.x += input.x * Time::deltaTime;
              camPos.y += input.y * Time::deltaTime;
              m_Camera.SetPosition(camPos);
          }, KeyCode::A, KeyCode::D, KeyCode::S, KeyCode::W),
          m_CameraRotationInputController([this](float input)
          {
              auto camRot = m_Camera.GetZRotation();
              camRot += input * Time::deltaTime;
              m_Camera.SetZRotation(camRot);
          }, KeyCode::Q, KeyCode::E)
    {}

    void OrthographicCameraController::OnUpdate()
    {
        if (EnableMovement)
        {
            m_CameraMovementInputController.OnUpdate();
        }
        if (EnableRotation)
        {
            m_CameraRotationInputController.OnUpdate();
        }
    }
    
    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrollEvent>(PKE_BIND_FUNCTION(OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(PKE_BIND_FUNCTION(OnWindowResized));
        
        m_CameraMovementInputController.OnEvent(e);
        m_CameraRotationInputController.OnEvent(e);
    }
    
    bool OrthographicCameraController::OnMouseScroll(MouseScrollEvent& e)
    {
        if (!EnableZoom) return false;
        
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(0.01f, m_ZoomLevel);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return true;
    }
    
    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        if (AspectRatioAdjustment == AspectRatioAdjustmentMode::None) return false;
        
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        
        if (AspectRatioAdjustment == AspectRatioAdjustmentMode::MatchHeight)
        {
            m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        }
        else
        {
            m_Camera.SetProjection(-m_ZoomLevel, m_ZoomLevel, -m_ZoomLevel / m_AspectRatio, m_ZoomLevel / m_AspectRatio);
        }
        
        return false;
    }
}
