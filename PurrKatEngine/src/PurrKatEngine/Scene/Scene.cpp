#include "pkepch.h"
#include "Scene.h"

#include "Components.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Renderer/Renderer2D/Renderer2D.h"

namespace PurrKatEngine
{
    Scene::Scene()
    {
        
    }

    Scene::~Scene() {}

    void Scene::OnUpdate()
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
        for (const auto& entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::DrawQuad(transform, nullptr, {1, 1}, sprite.Color);
            PKE_CORE_DEBUG("Rendering entity {} with transform: {}, {}, {}, {} | {}, {}, {}, {} | {}, {}, {}, {} | {}, {}, {}, {},", (uint32_t)entity,
                ((float*)&transform.Transform)[0],
                ((float*)&transform.Transform)[1],
                ((float*)&transform.Transform)[2],
                ((float*)&transform.Transform)[3],
                ((float*)&transform.Transform)[4],
                ((float*)&transform.Transform)[5],
                ((float*)&transform.Transform)[6],
                ((float*)&transform.Transform)[7],
                ((float*)&transform.Transform)[8],
                ((float*)&transform.Transform)[9],
                ((float*)&transform.Transform)[10],
                ((float*)&transform.Transform)[11],
                ((float*)&transform.Transform)[12],
                ((float*)&transform.Transform)[13],
                ((float*)&transform.Transform)[14],
                ((float*)&transform.Transform)[15],
                ((float*)&transform.Transform)[16]
                );
        }
    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }
}
