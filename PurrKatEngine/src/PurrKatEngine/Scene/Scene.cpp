#include "pkepch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"

namespace PurrKatEngine
{
    Scene::Scene() {}

    Scene::~Scene() {}

    Entity Scene::CreateEntity(const std::string& entityName)
    {
        Entity newEntity = {m_Registry.create(), this};
        newEntity.AddComponent<TransformComponent>();
        newEntity.AddComponent<TagComponent>(entityName);
        return newEntity;
    }

    void Scene::SetMainCamera(const Entity& camEntity)
    {
        if (camEntity.HasComponent<CameraComponent>() && camEntity.HasComponent<TransformComponent>())
        {
            m_MainCameraComponent = &camEntity.GetComponent<CameraComponent>();
            m_TransformCameraComponent = &camEntity.GetComponent<TransformComponent>();
        }
        else
        {
            PKE_CORE_ERROR("Scene::SetMainCamera: The provided entity does not have a CameraComponent or TransformComponent.");
        }
    }

    void Scene::OnUpdate()
    {
        // Update all Scriptable Entities.
        m_Registry.view<ScriptComponent>().each([this](auto entity, const ScriptComponent& scriptComp)
        {
            if (!scriptComp.Instance)
            {
                scriptComp.InstantiateScript();
                scriptComp.Instance->m_Entity = Entity{entity, this};
                scriptComp.Instance->OnStart();
            }
            
            scriptComp.Instance->OnUpdate();
        });
        
        // Find a camera if we don't have one.
        if (m_MainCameraComponent == nullptr)
            FindFirstCameraInScene();
        
        if (m_MainCameraComponent == nullptr)
            return;
        
        Renderer2D::BeginScene(*m_MainCameraComponent, *m_TransformCameraComponent);
        
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (const auto& entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::DrawQuad(transform, nullptr, {1, 1}, sprite.Color);
            // PKE_CORE_DEBUG("Rendering entity {} with transform:\n{}", (uint32_t)entity, to_string(transform.Transform));
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
            CameraComponent& cam = cameras.get<CameraComponent>(entity);
            m_MainCameraComponent = &cam;
            m_TransformCameraComponent = &m_Registry.get<TransformComponent>(entity);
            break;
        }
    }
}
