#pragma once
#include <glm/ext/matrix_transform.hpp>
#include "PurrKatEngine/Renderer/SceneCamera.h"

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
    
    struct TagComponent
    {
        std::string Tag = "Default";
        
        TagComponent() = default;
        TagComponent(std::string tag) : Tag(std::move(tag)) {}
        
        operator std::string& () { return Tag; }
        operator const std::string& () const { return Tag; }
    };
    
    struct SpriteComponent
    {
        glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
        
        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color) : Color(color) {}
        
        operator glm::vec4& () { return Color; }
        operator const glm::vec4& () const { return Color; }
    };
    
    struct CameraComponent
    {
        SceneCamera Camera;
        
        CameraComponent() = default;
        CameraComponent(SceneCamera camera) : Camera(std::move(camera)) {}
        
        operator PurrKatEngine::SceneCamera& () { return Camera; }
        operator const PurrKatEngine::SceneCamera& () const { return Camera; }
    };
}
