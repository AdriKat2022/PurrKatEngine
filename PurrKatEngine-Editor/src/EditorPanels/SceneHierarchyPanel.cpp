#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
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
        static bool inspectorOpened = true;
        ImGui::Begin("Inspector", &inspectorOpened);
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        if (entityToInspect.IsValid())
        {
            ImGui::Text("Inspecting Entity: %s (%u)", ENTITY_GET_NAME(entityToInspect).c_str(), (uint32_t)entityToInspect);
            
            DrawImGuiComponentsControllers(entityToInspect);
            
            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");
            
            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Camera Component"))
                {
                    entityToInspect.AddComponent<CameraComponent>().Camera.SetViewportSize(m_Scene->GetViewportWidth(), m_Scene->GetViewportHeight());
                    ImGui::CloseCurrentPopup();
                }
                
                if (ImGui::MenuItem("Sprite Component"))
                {
                    entityToInspect.AddComponent<SpriteComponent>();
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
                
        }
        else
            ImGui::Text("Select an entity to inspect it.");
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawImGuiComponentsControllers(const Entity& entityToInspect)
    {
        // TODO: Extract each component editor to their own definition.
        
        if (entityToInspect.HasComponent<TagComponent>())
        {
            TagComponent& tagComponent = entityToInspect.GetComponent<TagComponent>();
            
            static char buffer[50] = {};
            strcpy_s(buffer, tagComponent.Tag.c_str());
            ImGui::InputText("Game Entity Name", buffer, sizeof(buffer));
            tagComponent.Tag = buffer;
        }
        
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
                    if (ImGui::DragFloat("Near Clip", &nearClip, 0.01f, 0))
                        camComponent.SetPerspectiveNearClip(nearClip);
                    
                    float farClip = camComponent.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far Clip", &farClip, 0.01f))
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
                SpriteComponent& spriteComponent = e.GetComponent<SpriteComponent>();
                spriteComponent.Color = {1, 1, 1, 1};
            },
            true
            );
    }

    template<typename Component>
    void SceneHierarchyPanel::DrawComponent(const std::string& componentName, const Entity& entityToInspect, void (*imguiCode)(const Entity&), void (*onReset)(const Entity&), bool allowRemove)
    {
        if (!entityToInspect.HasComponent<Component>())
            return;
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        
        if (ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, componentName.c_str()))
        {
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

            bool toRemove = false;

            if (ImGui::Button("+", ImVec2(20, 20)))
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

                ImGui::EndPopup();
            }

            imguiCode(entityToInspect);

            if (toRemove)
                entityToInspect.RemoveComponent<Component>();

            ImGui::TreePop();
        }
        
        ImGui::PopStyleVar();
    }
}
