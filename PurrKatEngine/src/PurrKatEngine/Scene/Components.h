#pragma once
#include <string>
#include <glm/ext/matrix_transform.hpp>
#include "PurrKatEngine/Renderer/SceneCamera.h"

namespace PurrKatEngine
{
    class Entity;
    class ScriptableEntity;

    struct TransformComponent
    {
        glm::vec3 Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
        
        TransformComponent() = default;
        TransformComponent(const glm::vec3& position) : Position(position) {}
        TransformComponent(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation) : Position(position), Rotation(rotation), Scale(size) {}
        
        glm::mat4 GetTransformMatrix() const
        {
            auto rotation =
                  glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
                * glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
                * glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});
            
            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
        
        operator glm::mat4 () const { return GetTransformMatrix(); }
    };
    
    struct TagComponent
    {
        std::string Tag = "GameEntity";
        
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
        
        void OnMount(Entity& entity);
        
        operator PurrKatEngine::SceneCamera& () { return Camera; }
        operator const PurrKatEngine::SceneCamera& () const { return Camera; }
    };
    
    struct ScriptComponent
    {
        ScriptableEntity* Instance = nullptr;
        
        // Function pointers (saves the allocation of std::function).
        ScriptableEntity*   (*InstantiateScript)    ();
        void                (*DestroyScript)        (ScriptComponent&);
        
        template<typename T>
        void Bind()
        {
            InstantiateScript = [] { return (ScriptableEntity*)new T(); };
            DestroyScript = [](ScriptComponent& c) { delete (T*)c.Instance; c.Instance = nullptr; };
        }
    };
}
