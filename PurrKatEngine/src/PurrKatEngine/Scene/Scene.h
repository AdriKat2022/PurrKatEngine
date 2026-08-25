#pragma once

#include <entt.h>

namespace PurrKatEngine
{
    struct TransformComponent;
    struct CameraComponent;

    class Scene
    {
        friend class Entity;
        
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& entityName = {});
        
        void SetMainCamera(const Entity& camEntity);
        void OnUpdate();
        void OnViewportResize(uint32_t width, uint32_t height);

    private:
        void FindFirstCameraInScene();
        
    private:
        uint32_t m_ViewportWidth;
        uint32_t m_ViewportHeight;
        
        entt::registry m_Registry;
        CameraComponent* m_MainCameraComponent = nullptr;
        TransformComponent* m_TransformCameraComponent = nullptr;
    };
}
