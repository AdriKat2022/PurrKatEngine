#pragma once
#include "PurrKatEngine/Scene/Entity.h"

namespace PurrKatEngine
{
    class Scene;

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel();
        SceneHierarchyPanel(Scene* scene);

        void SetScene(Scene* scene);
        void OnImGuiRender();
        
    private:
        void DrawImGuiSceneHierarchy();
        void DrawEntityNode(Entity& entity);
        
        void DrawImGuiInspectorOfEntity(Entity& entityToInspect) const;
        
        static void DrawImGuiComponentsControllers(const Entity& entityToInspect);
        
        template <class Component>
        static void DrawComponent(const std::string& componentName, const Entity& entityToInspect, void (*imguiCode)(const Entity&), void (*onReset)(const Entity&), bool allowRemove);

    private:
        Scene* m_Scene;
        Entity m_ActiveSelection = {};
    };
}
