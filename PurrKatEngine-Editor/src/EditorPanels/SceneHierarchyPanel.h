#pragma once
#include "PurrKatEngine/Core.h"

namespace PurrKatEngine
{
    class Scene;

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel();
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);
        void OnImGuiRender();
        
    private:
        Ref<Scene> m_Scene;
    };
}
