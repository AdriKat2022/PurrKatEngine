#pragma once

namespace PurrKatEngine
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
        void SetZRotation(float rotation) { m_Rotation.z = rotation; RecalculateViewMatrix(); }

        void SetProjection(float x, float x1, float x2, float x3);

        float* GetZoomPtr() { return &m_ProjectionMatrix[1][1]; }
        
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetRotation() const { return m_Rotation; }
        float GetZRotation() const { return m_Rotation.z; }

        glm::vec3 ScreenToWorldPosition(const glm::vec2& screenPosition) const;
        
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();
        
    private:
        glm::mat4 m_ProjectionMatrix; // How the camera will render (orthographic == 
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix; // Cached multiplied projection and view matrixes
        
        glm::vec3 m_Position = { 0, 0, 0 };
        glm::vec3 m_Rotation = { 0, 0, 0 };
    };
}
