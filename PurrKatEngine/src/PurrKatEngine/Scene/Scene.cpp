#include "pkepch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

namespace PurrKatEngine
{
    Scene::Scene() {}

    Scene::~Scene() {}

    Entity Scene::CreateEntity(const std::string& entityName, const glm::vec3& position)
    {
        Entity newEntity = {m_Registry.create(), this};
        newEntity.AddComponent<TransformComponent>(position);
        newEntity.AddComponent<TagComponent>(entityName);
        return newEntity;
    }

    void Scene::DestroyEntity(Entity& entity)
    {
        m_Registry.destroy(entity);
        entity.Invalidate();
    }

    void Scene::SetMainCamera(const Entity& camEntity)
    {
        if (camEntity.HasComponent<CameraComponent>() && camEntity.HasComponent<TransformComponent>())
        {
            m_MainCameraEntityRef = camEntity;
        }
        else
        {
            PKE_CORE_ERROR("Scene::SetMainCamera: The provided entity does not have a CameraComponent or TransformComponent.");
        }
    }

    Entity Scene::GetMainCamera() { return {m_MainCameraEntityRef, this}; }

    void Scene::OnUpdate()
    {
        // Update all Scriptable Entities.
        m_Registry.view<ScriptComponent>().each([this](auto entity, ScriptComponent& scriptComp)
        {
            if (!scriptComp.Instance)
            {
                scriptComp.Instance = scriptComp.InstantiateScript();
                scriptComp.Instance->m_Entity = Entity{entity, this};
                scriptComp.Instance->OnStart();
            }
            
            scriptComp.Instance->OnUpdate();
        });
        
        // Find a camera if we don't have one.
        if (m_MainCameraEntityRef == entt::null || !m_Registry.all_of<TransformComponent, CameraComponent>(m_MainCameraEntityRef))
            FindFirstCameraInScene();
        
        if (m_MainCameraEntityRef == entt::null || !m_Registry.all_of<TransformComponent, CameraComponent>(m_MainCameraEntityRef))
        {
            PKE_CORE_WARN("There are no valid camera in the scene!");
            return;
        }
        
        auto& camera = m_Registry.get<CameraComponent>(m_MainCameraEntityRef);
        auto& transformComponent = m_Registry.get<TransformComponent>(m_MainCameraEntityRef);
        
        Renderer2D::BeginScene(camera, transformComponent);
        
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (const auto& entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::DrawQuad(transform, nullptr, {1, 1}, sprite.Color);
        }
        
        Renderer2D::EndScene();
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        
        auto cameras = m_Registry.view<CameraComponent>();
        for (auto entity : cameras)
        {
            auto& cam = cameras.get<CameraComponent>(entity);
            cam.Camera.SetViewportSize(width, height);
        }
    }

    void Scene::FindFirstCameraInScene()
    {
        auto cameras = m_Registry.view<CameraComponent>();
        for (entt::entity entity : cameras)
        {
            Entity camEntity = {entity, this};
            if (camEntity.HasComponent<CameraComponent>() && camEntity.HasComponent<TransformComponent>())
            {
                m_MainCameraEntityRef = camEntity;
                break;
            }
        }
    }
}
