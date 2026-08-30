#pragma once
#include "Components.h"
#include "Scene.h"

#define ENTITY_GET_NAME(entity) entity.GetComponent<TagComponent>().Tag

namespace PurrKatEngine
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene) : m_EntityId(handle), m_Scene(scene) { }
        
        const std::string& GetName() const { return GetComponent<TagComponent>(); }
        
        Scene* GetScene() const { return m_Scene; }
        
        void Destroy() { m_Scene->DestroyEntity(*this); }
        
        template<typename T>
        bool HasComponent() const
        {
            return m_Scene->m_Registry.any_of<T>(m_EntityId);
        }
        
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            auto& component = m_Scene->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
            OnComponentAdded(component);
            return component;
        }
        
        template<typename T>
        T& GetComponent() const
        {
            return m_Scene->m_Registry.get<T>(m_EntityId);
        }
        
        template<typename T>
        bool TryGetComponent(const T*& outComponent) const
        {
            if (m_Scene->m_Registry.any_of<T>(m_EntityId))
            {
                outComponent = &m_Scene->m_Registry.get<T>(m_EntityId);
                return true;
            }
            
            outComponent = nullptr;
            return false;
        }
        
        template<typename T>
        void RemoveComponent() const
        {
            m_Scene->m_Registry.remove<T>(m_EntityId);
        }
        
        bool IsValid() const { return m_EntityId != entt::null && m_Scene->m_Registry.all_of<TransformComponent>(m_EntityId); }
        
        operator bool() const { return IsValid(); }
        operator uint32_t() const { return (uint32_t)m_EntityId; }
        operator uint64_t() const { return (uint64_t)m_EntityId; }
        operator entt::entity() const { return m_EntityId; }
        
        bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return m_EntityId != other.m_EntityId || m_Scene != other.m_Scene; }

    private:
        void Invalidate() { m_EntityId = { entt::null }; }
        
        template<typename T>
        void OnComponentAdded(T& component)
        {
            if constexpr (requires { component.OnMount(*this); })
                component.OnMount(*this);
        }
        
    private:
        entt::entity m_EntityId = {entt::null };
        Scene* m_Scene = nullptr;
        
        friend class Scene;
    };
}
