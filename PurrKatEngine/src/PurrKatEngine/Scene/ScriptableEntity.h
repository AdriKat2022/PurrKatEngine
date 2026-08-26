#pragma once
#include "Entity.h"

namespace PurrKatEngine
{
    class ScriptableEntity
    {
        friend class Scene;
        
    public:
        virtual ~ScriptableEntity() = default;
        
        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnDestroy() {}
        
        // virtual void OnImGuiRender() {}
        
        template<typename T>
        T& GetComponent() { return m_Entity.GetComponent<T>(); }
        
        template<typename T>
        bool HasComponent() const { return m_Entity.HasComponent<T>(); }
        
    protected:
        Entity m_Entity;
    };
}
