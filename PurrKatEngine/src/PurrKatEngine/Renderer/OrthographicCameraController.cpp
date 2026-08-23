#include "pkepch.h"
#include "OrthographicCameraController.h"

#include "PurrKatEngine/Inputs/Input.h"
#include "PurrKatEngine/Inputs/Time.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

namespace PurrKatEngine
{
    OrthographicCameraController::OrthographicCameraController()
        : m_CameraBounds({.Left = -m_AspectRatio * m_ZoomLevel, .Right = m_AspectRatio * m_ZoomLevel, .Top = m_ZoomLevel, .Bottom = -m_ZoomLevel}),
          m_Camera(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top)
    {}

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel, bool useScrollToZoom)
        : EnableZoom(useScrollToZoom),
          m_AspectRatio(aspectRatio),
          m_ZoomLevel(zoomLevel),
          m_CameraBounds({.Left = -m_AspectRatio * m_ZoomLevel, .Right = m_AspectRatio * m_ZoomLevel, .Top = m_ZoomLevel, .Bottom = -m_ZoomLevel}),
          m_Camera(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top)
    {}

    void OrthographicCameraController::OnUpdate()
    {
        if (EnableMovement)
            HandleMovement();
        
        if (EnableRotation)
            HandleRotation();
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrollEvent>(PKE_BIND_FUNCTION(OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(PKE_BIND_FUNCTION(OnWindowResized));
    }

    void OrthographicCameraController::HandleMovement()
    {
        auto camPos = m_Camera.GetPosition();
        auto input = Input::GetAxis2D(KeyCode::W, KeyCode::A, KeyCode::S, KeyCode::D);
        
        camPos.x += (
            cos(m_CameraRotation) * input.x
            -sin(m_CameraRotation) * input.y
        ) * (float)Time::deltaTime * m_ZoomLevel;
        
        camPos.y += (
            cos(m_CameraRotation) * input.y +
            sin(m_CameraRotation) * input.x
        ) * (float)Time::deltaTime * m_ZoomLevel;
              
        m_Camera.SetPosition(camPos);
    }
    
    void OrthographicCameraController::HandleRotation()
    {
        auto input = Input::GetAxis(KeyCode::Q, KeyCode::E);
        m_CameraRotation += input * (float)Time::deltaTime;
        m_Camera.SetZRotation(m_CameraRotation);
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrollEvent& e)
    {
        if (!EnableZoom) return false;
        
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(0.01f, m_ZoomLevel);
        UpdateCameraProjection();
        return true;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        if (AspectRatioAdjustment == AspectRatioAdjustmentMode::None) return false;
        
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        
        UpdateCameraProjection();
        
        return false;
    }

    void OrthographicCameraController::UpdateCameraProjection()
    {
        if (AspectRatioAdjustment == AspectRatioAdjustmentMode::MatchHeight)
        {
            m_CameraBounds = {.Left = -m_AspectRatio * m_ZoomLevel, .Right = m_AspectRatio * m_ZoomLevel, .Top = m_ZoomLevel, .Bottom = -m_ZoomLevel};
        }
        else
        {
            m_CameraBounds = {.Left = -m_ZoomLevel, .Right = m_ZoomLevel, .Top = m_ZoomLevel / m_AspectRatio, .Bottom = -m_ZoomLevel / m_AspectRatio};
        }
        
        m_Camera.SetProjection(m_CameraBounds.Left, m_CameraBounds.Right, m_CameraBounds.Bottom, m_CameraBounds.Top);
    }
}
