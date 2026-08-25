#pragma once
#include <entt.h>
#include "Scene.h"

#define ENTITY_GET_NAME(entity) entity.GetComponent<TagComponent>().Tag

namespace PurrKatEngine
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene) : m_EntityId(handle), m_Scene(scene) {}
        
        template<typename T>
        bool HasComponent() const
        {
            return m_Scene->m_Registry.any_of<T>(m_EntityId);
        }
        
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            return m_Scene->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
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
        
        operator bool() const { return m_EntityId != entt::null; }
        operator uint32_t() const { return (uint32_t)m_EntityId; }
        operator uint64_t() const { return (uint64_t)m_EntityId; }
        
        bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return m_EntityId != other.m_EntityId || m_Scene != other.m_Scene; }
        
    private:
        entt::entity m_EntityId = {entt::null};
        Scene* m_Scene;
    };
}
