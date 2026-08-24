#pragma once

#include <spdlog/details/registry.h>
#include "entt.h"

namespace PurrKatEngine
{
    class Scene
    {
    public:
        Scene();
        ~Scene();
        
        entt::registry& GetRegistry() { return m_Registry; }
        entt::entity CreateEntity();
        
        void OnUpdate();
        
    private:
        entt::registry m_Registry;
    };
}
