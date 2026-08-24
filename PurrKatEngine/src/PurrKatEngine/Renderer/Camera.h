#pragma once

namespace PurrKatEngine
{
    // A runtime lightweight camera.
    class Camera
    {
    public:
        Camera() {}
        Camera(const glm::mat4& mat) : m_ProjectionMatrix(mat) {}
        virtual ~Camera() {}

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        
    protected:
        glm::mat4 m_ProjectionMatrix;
    };
}
