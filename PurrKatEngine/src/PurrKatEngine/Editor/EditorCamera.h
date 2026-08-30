#pragma once
#include "PurrKatEngine/Events/Event.h"
#include "PurrKatEngine/Events/MouseScrollEvent.h"
#include "PurrKatEngine/Renderer/Camera.h"

namespace PurrKatEngine
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate();
        void OnEvent(Event& event);
        
        void SetFocusPoint(const glm::vec3& focalPoint) { m_FocalPoint = focalPoint; UpdateView(); }
        
        float GetDistance() const { return m_Distance; }
        void SetDistance(float distance) { m_Distance = distance; }
        
        void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        const glm::mat4& GetViewMatrix()  const { return m_ViewMatrix; }
        glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
        
        // Directions
        const glm::vec3& GetPosition() const { return m_Position; }
        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;
        glm::quat GetOrientation() const;
        
        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }
        
    private:
        void UpdateProjection();
        void UpdateView();
        
        bool OnMouseScroll(MouseScrollEvent& e);
        
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);
        
        glm::vec3 CalculatePosition() const;
        
        std::pair<float, float> PanSpeed() const;
        float ZoomSpeed() const;
        float RotationSpeed() const;
        
    private:
        glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};
        
        // View parameters
        float m_FOV = 45.0f;
        float m_AspectRatio = 1.778f;
        float m_NearClip = 0.1f;
        float m_FarClip = 1000.0f;
        float m_Distance = 10.0f;
        float m_Pitch = 0.0f;
        float m_Yaw = 0.0f;

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};
        
        float m_ViewportWidth = 1280, m_ViewportHeight = 720;
        
        glm::mat4 m_ViewMatrix;
    };
}
