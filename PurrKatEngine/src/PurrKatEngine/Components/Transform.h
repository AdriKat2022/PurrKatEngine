#pragma once
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace PurrKatEngine
{
    class Transform
    {
    public:
        Transform() = default;
        
        void Move(glm::vec3 positionShift) { m_Position += positionShift; }
        
        void SetRotation(glm::vec3 newRotation) { m_Rotation = newRotation; }
        void SetScale(glm::vec3 newScale) { m_Scale = newScale; }
        
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetRotation() const { return m_Rotation; }
        const glm::vec3& GetScale() const { return m_Scale; }
        
        glm::mat4 GetTransformMatrix() const { return glm::translate(glm::mat4(1.0f), m_Position) * glm::scale(glm::mat4(1.0f), m_Scale); }
        
    private:
        glm::vec3 m_Position{0, 0, 0};
        glm::vec3 m_Rotation{0, 0, 0};
        glm::vec3 m_Scale{1, 1, 1};
    };
}
