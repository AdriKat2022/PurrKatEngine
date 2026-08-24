#pragma once
#include <glm/detail/type_quat.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace PurrKatEngine
{
    class Transform
    {
    public:
        static glm::mat4 CalculateTransformMatrix2D(const glm::vec3& position, const glm::vec2& scale);
        static glm::mat4 CalculateTransformMatrix2D(const glm::vec3& position, float rotation, const glm::vec2& scale);
        
    public:
        Transform() = default;
        
        void Move(glm::vec3 positionShift) { Position += positionShift; }
        
        void SetRotation(glm::vec3 newRotation) { Rotation = newRotation; }
        void SetScale(glm::vec3 newScale) { Scale = newScale; }
        
        const glm::vec3& GetPosition() const { return Position; }
        const glm::vec3& GetRotation() const { return Rotation; }
        const glm::vec3& GetScale() const { return Scale; }
        
        glm::mat4 GetTransformMatrix() const { return glm::translate(glm::mat4(1.0f), Position) * glm::scale(glm::mat4(1.0f), Scale); }
        
    public:
        glm::vec3 Position{0, 0, 0};
        glm::vec3 Rotation{0, 0, 0};
        glm::vec3 Scale{1, 1, 1};
    };
}
