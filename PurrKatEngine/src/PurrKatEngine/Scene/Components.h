#pragma once
#include <glm/ext/matrix_transform.hpp>

namespace PurrKatEngine
{
    struct TransformComponent
    {
        glm::mat4 Transform = {glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1))};
        
        TransformComponent() = default;
        TransformComponent(const glm::mat4& transform) : Transform(transform) {}
        
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };
    
    struct SpriteComponent
    {
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
        
        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color) : Color(color) {}
        
        operator glm::vec4& () { return Color; }
        operator const glm::vec4& () const { return Color; }
    };
}
