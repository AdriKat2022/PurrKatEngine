#pragma once
#include <entt.h>
#include "Scene.h"

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
        T& AddComponent(Args&&... args) const
        {
            return m_Scene->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
        }
        
        template<typename T>
        T& GetComponent() const
        {
            return m_Scene->m_Registry.get<T>(m_EntityId);
        }
        
        template<typename T>
        bool TryGetComponent(T& outComponent) const
        {
            if (m_Scene->m_Registry.any_of<T>(m_EntityId))
            {
                outComponent = &m_Scene->m_Registry.get<T>(m_EntityId);
                return true;
            }

            return false;
        }

    private:
        entt::entity m_EntityId = {entt::null};
        Scene* m_Scene;
    };
}
