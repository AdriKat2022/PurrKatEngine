#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_internal.h"
#include "PurrKatEngine/Scene/Scene.h"
#include "PurrKatEngine/Scene/Components.h"
#include "PurrKatEngine/Utility/ImGuiUtility.h"

namespace PurrKatEngine
{
    SceneHierarchyPanel::SceneHierarchyPanel() = default;
    
    SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene): m_Scene(scene) {}
    
    void SceneHierarchyPanel::SetScene(Scene* scene) { m_Scene = scene; }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        DrawImGuiSceneHierarchy();
        DrawImGuiInspectorOfEntity(m_ActiveSelection);
    }

    void SceneHierarchyPanel::DrawImGuiSceneHierarchy()
    {
        ImGui::Begin("Scene Hierarchy");
        
        // Context Menu (right click in blank space only)
        if (ImGui::BeginPopupContextWindow(nullptr, 1))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_ActiveSelection = m_Scene->CreateEntity("Game Entity");
            
            ImGui::EndPopup();
        }
        
        for(entt::entity entityId : m_Scene->m_Registry.view<entt::entity>())
        {
            Entity entity = {entityId, m_Scene};
            DrawEntityNode(entity);
        }
        
        ImGui::End();
    }
    
    void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
    {
        const std::string& entityName = entity.GetName();

        int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (m_ActiveSelection == entity)
            flags |= ImGuiTreeNodeFlags_Selected;
        
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, entityName.c_str());
        
        if (ImGui::IsItemClicked())
            m_ActiveSelection = entity;
        
        bool deleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                deleted = true;
            ImGui::EndPopup();
        }
        
        if (opened)
        {
            // TODO: Draw children entities here.
            
            opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, entityName.c_str());
            
            if (opened)
                ImGui::TreePop();
            
            ImGui::TreePop();
        }
        
        if (deleted)
        {
            if (m_ActiveSelection == entity)
                m_ActiveSelection = {};
            entity.Destroy();
        }
    }

    void SceneHierarchyPanel::DrawImGuiInspectorOfEntity(Entity& entityToInspect) const
    {
        ImGui::ShowDemoWindow();
        
        static bool inspectorOpened = true;
        // std::string windowName = entityToInspect.IsValid() ? "Inspector: " + entityToInspect.GetName() : "Inspector";
        ImGui::Begin("Inspector", &inspectorOpened);
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        if (entityToInspect.IsValid())
        {
            float buttonWidth = std::min(150.0f, ImGui::GetContentRegionAvail().x * 0.4f);
            float inputWidth = ImGui::GetContentRegionAvail().x - buttonWidth - ImGui::GetStyle().ItemSpacing.x;
            
            if (entityToInspect.HasComponent<TagComponent>())
            {
                TagComponent& tagComponent = entityToInspect.GetComponent<TagComponent>();
            
                ImGui::SetNextItemWidth(inputWidth);
                
                static char buffer[50] = {};
                strcpy_s(buffer, tagComponent.Tag.c_str());
                if (ImGui::InputText("##Game Entity Name", buffer, sizeof(buffer)))
                    tagComponent.Tag = buffer;
            }
            
            ImGui::SameLine();
            
            // ImGui::SetNextItemWidth(buttonWidth);
            if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0)))
                ImGui::OpenPopup("AddComponent");
            
            if (ImGui::BeginPopup("AddComponent"))
            {
                DrawAddComponentItem<CameraComponent>("Camera Component", entityToInspect);
                
                DrawAddComponentItem<SpriteComponent>("Sprite Component", entityToInspect);
                
                ImGui::EndPopup();
            }
            
            DrawImGuiComponentsControllers(entityToInspect);
        }
        else
            ImGui::Text("Select an entity to inspect it.");
        
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawImGuiComponentsControllers(const Entity& entityToInspect)
    {
        // TODO: Extract each component editor to their own definition.
        
        DrawComponent<TransformComponent>("Transform Component", entityToInspect,
            [](const Entity& e)
            {
                TransformComponent& transform = e.GetComponent<TransformComponent>();
                ImGuiUtility::DrawVec3Control("Position", transform.Position);
                glm::vec3 rotDegree = glm::degrees(transform.Rotation);
                ImGuiUtility::DrawVec3Control("Rotation", rotDegree);
                transform.Rotation = glm::radians(rotDegree);
                ImGuiUtility::DrawVec3Control("Scale", transform.Scale, 1);
            },
            [](const Entity& e)
            {
                TransformComponent& transform = e.GetComponent<TransformComponent>();
                transform.Position = {0, 0, 0};
                transform.Rotation = {0, 0, 0};
                transform.Scale = {1, 1, 1};
            },
            false
            );
        
        DrawComponent<SpriteComponent>("Sprite 2D Component", entityToInspect,
            [](const Entity& e)
            {
                SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(spriteComponent.Color));
            },
            [](const Entity& e)
            {
                SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
                spriteComponent.Color = {1, 1, 1, 1};
            },
            true
            );
        
        DrawComponent<CameraComponent>("Camera Component", entityToInspect,
            [](const Entity& e)
            {
                SceneCamera& camComponent = e.GetComponent<CameraComponent>();
                
                int projectionType = (int)camComponent.GetProjectionType(); 

                if (ImGui::RadioButton("Perspective", &projectionType, (int)SceneCamera::ProjectionType::Perspective))
                    camComponent.SetProjectionType((SceneCamera::ProjectionType)projectionType);

                ImGui::SameLine();
                
                if (ImGui::RadioButton("Orthographic", &projectionType, (int)SceneCamera::ProjectionType::Orthographic))
                    camComponent.SetProjectionType((SceneCamera::ProjectionType)projectionType);
                
                if (projectionType == (int)SceneCamera::ProjectionType::Perspective)
                {
                    float pov = glm::degrees(camComponent.GetPerspectivePov());
                    if (ImGui::DragFloat("POV", &pov, 0.01f))
                        camComponent.SetPerspectivePov(glm::radians(pov));
                    
                    float nearClip = camComponent.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near Clip", &nearClip, 0.01f, 0, FLT_MAX))
                        camComponent.SetPerspectiveNearClip(nearClip);
                    
                    float farClip = camComponent.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far Clip", &farClip, 0.01f, 0, FLT_MAX))
                        camComponent.SetPerspectiveFarClip(farClip);
                }
                else
                {
                    float size = camComponent.GetOrthographicSize();
                    if (ImGui::DragFloat("Orthographic Size", &size, 0.01f))
                        camComponent.SetOrthographicSize(size);
                    
                    float nearClip = camComponent.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near Clip", &nearClip, 0.01f))
                        camComponent.SetOrthographicNearClip(nearClip);
                    
                    float farClip = camComponent.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far Clip", &farClip, 0.01f))
                        camComponent.SetOrthographicFarClip(farClip);
                }
                
                // ******** ASPECT RATIO ******* //
                
                const char* aspectRatioModes[] = {
                    "Variable: Match View",
                    "Fixed: Match Width",
                    "Fixed: Match Height"
                };
                
                int mode = (int)camComponent.GetAspectRatioAdjustementMode();

                if (ImGui::Combo("Aspect Ratio", &mode, aspectRatioModes, IM_ARRAYSIZE(aspectRatioModes)))
                    camComponent.SetAspectRatioAdjustementMode((SceneCamera::AspectRatioAdjustmentMode)mode);
            },
            [](const Entity& e)
            {
                SceneCamera& camComponent = e.GetComponent<CameraComponent>();
                camComponent.SetProjectionType(SceneCamera::ProjectionType::Perspective);
                camComponent.SetAspectRatioAdjustementMode(SceneCamera::AspectRatioAdjustmentMode::MatchHeight);
                camComponent.SetPerspective(glm::radians(45.0f), 0.01f, 1000.0f);
            },
            true
            );
        
        DrawComponent<ScriptComponent>("Custom Script Component", entityToInspect,
            [](const Entity& e) {},
            [](const Entity& e) {},
            true
            );
    }

    template<typename Component>
    void SceneHierarchyPanel::DrawComponent(const std::string& componentName, const Entity& entityToInspect, void (*imguiCode)(const Entity&), void (*onReset)(const Entity&), bool allowRemove)
    {
        if (!entityToInspect.HasComponent<Component>())
            return;
        
        // Add some spacing for all components after the TransformComponent to visually separate them.
        if constexpr (!std::is_same_v<Component, TransformComponent>)
            ImGui::Spacing();
        
        ImGui::PushID((int)typeid(Component).hash_code());
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

        auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth;
        
        bool opened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), flags, "%s", componentName.c_str());
        
        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        if (opened)
            ImGui::Unindent();
        
        // ------ ITEM MENU (on the right side of the component header) -------
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
        
        bool toRemove = false;
        
        if (ImGui::Button("..", ImVec2(lineHeight, lineHeight)))
            ImGui::OpenPopup("ComponentSettings");

        if (ImGui::BeginPopup("ComponentSettings"))
        {
            ImGui::BeginDisabled(onReset == nullptr);
            if (ImGui::MenuItem("Reset"))
                onReset(entityToInspect);
            ImGui::EndDisabled();

            ImGui::BeginDisabled(!allowRemove); // Cannot remove transform component.
            if (ImGui::MenuItem("Remove Component"))
                toRemove = true;
            ImGui::EndDisabled();
            if (!allowRemove)
                ImGui::SetItemTooltip("Removing component '%s' is not allowed.", componentName.c_str());

            ImGui::EndPopup();
        }
        
        if (opened)
            ImGui::Indent();
        
        if (opened)
        {
            imguiCode(entityToInspect);
            ImGui::TreePop();
        }
        
        if (toRemove)
            entityToInspect.RemoveComponent<Component>();
        
        ImGui::PopStyleVar();
        ImGui::PopID();
    }
    
    template<typename Component>
    void SceneHierarchyPanel::DrawAddComponentItem(const std::string& componentName, Entity& entity, void (*onAdd)(const Entity&, Component& component), bool allowMultiple) const
    {
        bool hasComponent = entity.HasComponent<Component>();
        
        ImGui::PushID((int)typeid(Component).hash_code());
        ImGui::BeginDisabled(hasComponent);
        {
            if (ImGui::MenuItem(componentName.c_str()))
            {
                Component& component = entity.AddComponent<Component>();
                if (onAdd != nullptr)
                    onAdd(entity, component);
                ImGui::CloseCurrentPopup();
            }
        
            if (hasComponent && !allowMultiple)
                ImGui::SetItemTooltip("Each entity can only have one '%s'.", componentName.c_str());
        }
        ImGui::EndDisabled();
        ImGui::PopID();
    }
}
