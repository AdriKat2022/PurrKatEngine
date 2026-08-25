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
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        Scene* m_Scene;
        Entity m_ActiveSelection;
    };
}
