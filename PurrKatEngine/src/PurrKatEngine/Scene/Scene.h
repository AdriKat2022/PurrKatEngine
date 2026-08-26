#pragma once

#include <entt.h>

namespace PurrKatEngine
{
    struct TransformComponent;
    struct CameraComponent;

    class Scene
    {
        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;
        
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& entityName = "Game Entity", const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
        void DestroyEntity(Entity& entity);
        
        void OnUpdate();
        void OnViewportResize(uint32_t width, uint32_t height);
        
        void SetMainCamera(const Entity& camEntity);
        Entity GetMainCamera();
        
        uint32_t GetViewportWidth() const { return m_ViewportWidth; }
        uint32_t GetViewportHeight() const { return m_ViewportHeight; }
        
        void EmptyScene();

    private:
        void FindFirstCameraInScene();
        
    private:
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;
        
        entt::registry m_Registry;
        entt::entity m_MainCameraEntityRef = entt::null;
    };
}
