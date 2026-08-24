#include "pkepch.h"
#include "Transform.h"

namespace PurrKatEngine
{
    glm::mat4 Transform::CalculateTransformMatrix2D(const glm::vec3& position, const glm::vec2& scale)
    {
        return glm::translate(glm::mat4(1.0f), position)
            * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});
    }
    
    glm::mat4 Transform::CalculateTransformMatrix2D(const glm::vec3& position, float rotation, const glm::vec2& scale)
    {
        return glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});
    }
}
