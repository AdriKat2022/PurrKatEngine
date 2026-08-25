#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "PurrKatEngine/Scene/Scene.h"
#include "PurrKatEngine/Scene/Components.h"

namespace PurrKatEngine
{
    SceneHierarchyPanel::SceneHierarchyPanel() = default;
    SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene): m_Scene(scene) {}
    
    void SceneHierarchyPanel::SetScene(Scene* scene) { m_Scene = scene; }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        static bool sceneHierarchyPanelOpened = true;
        ImGui::Begin("Scene Hierarchy", &sceneHierarchyPanelOpened);
        
        if (sceneHierarchyPanelOpened)
        {
            for(entt::entity entity : m_Scene->m_Registry.view<entt::entity>())
            {
                DrawEntityNode({entity, m_Scene});
            }
        }
        
        ImGui::End();
        
        static bool inspectorOpened = true;
        ImGui::Begin("Inspector", &inspectorOpened);
        DrawComponents(m_ActiveSelection);
        ImGui::End();
        
        ImGui::ShowDemoWindow();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        std::string& entityName = entity.GetComponent<TagComponent>();

        int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (m_ActiveSelection == entity)
            flags |= ImGuiTreeNodeFlags_Selected;
        
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, entityName.c_str());
        
        if (ImGui::IsItemClicked())
            m_ActiveSelection = entity;
        
        if (opened)
        {
            opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, entityName.c_str());
            
            if (opened)
                ImGui::TreePop();
            
            ImGui::TreePop();
        }
        
        // ImGui::Text("%s", entityName.c_str());
    }
    
    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        // TODO: Extract each component editor to their own definition.
        if (!entity)
        {
            ImGui::Text("Select an entity to inspect it.");
            return;
        }
        
        if (entity.HasComponent<TagComponent>())
        {
            TagComponent& tagComponent = entity.GetComponent<TagComponent>();
            
            static char buffer[50] = {};
            strcpy_s(buffer, tagComponent.Tag.c_str());
            ImGui::InputText("Game Entity Name", buffer, sizeof(buffer));
            tagComponent.Tag = buffer;
        }
        
        if (entity.HasComponent<TransformComponent>() && ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            TransformComponent& transform = entity.GetComponent<TransformComponent>();
            ImGui::DragFloat3("Position", glm::value_ptr(transform.Transform[3]), 0.01f);
        }
        
        if (entity.HasComponent<SpriteComponent>() && ImGui::CollapsingHeader("Sprite 2D Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            SpriteComponent& spriteComponent = entity.GetComponent<SpriteComponent>();
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteComponent.Color));
        }
        
        if (entity.HasComponent<CameraComponent>() && ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
        {
            SceneCamera& camComponent = entity.GetComponent<CameraComponent>();
            
            const char* modes[] = {
                "Variable: Match View",
                "Fixed: Match Width",
                "Fixed: Match Height"
            };
            
            float size = camComponent.GetOrthographicSize();
            if (ImGui::DragFloat("Camera Size", &size, 0.01f))
                camComponent.SetOrthographicSize(size);
            
            int mode = (int)camComponent.GetAspectRatioAdjustementMode();

            if (ImGui::Combo("Aspect Ratio", &mode, modes, IM_ARRAYSIZE(modes)))
                camComponent.SetAspectRatioAdjustementMode((SceneCamera::AspectRatioAdjustmentMode)mode);
        }
    }
}
