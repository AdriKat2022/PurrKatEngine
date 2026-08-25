#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "PurrKatEngine/Scene/Scene.h"
#include "PurrKatEngine/Scene/Components.h"

namespace PurrKatEngine
{
    SceneHierarchyPanel::SceneHierarchyPanel() = default;
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene): m_Scene(scene) {}
    
    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene) { m_Scene = scene; }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        static bool sceneHierarchyPanelOpened = true;
        ImGui::Begin("Scene Hierarchy", &sceneHierarchyPanelOpened);
        
        if (sceneHierarchyPanelOpened)
            for(entt::entity entity : m_Scene->m_Registry.view<entt::entity>())
            {
                const std::string& entityName = m_Scene->m_Registry.get<TagComponent>(entity);
                ImGui::Text("%s", entityName.c_str());
            }
        
        ImGui::End();
    }
}
