#include "EditorViewLayer.h"

#include "PurrKatEngine/Controls/Controls.h"
#include "PurrKatEngine/Serialization/SceneSerializer.h"
#include "PurrKatEngine/Scene/Scene.h"

namespace PurrKatEngine
{
    EditorViewLayer::EditorViewLayer()
    {
        // m_GrassSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Grass.png", {.Filter = Texture2D::FilterType::Nearest}));
        // m_GrassSpriteSheet.SetSpriteSheetOptions({.CellCount = {11, 7}});
        
        // m_UpScaledFrameBuffer = FrameBuffer::CreateRef({
        //     .Width = 1280,
        //     .Height = 720,
        //     .AttachmentsSpecs = {
        //         { FrameBufferTextureFormat::RGBA8, ImageFilterType::Nearest },
        //         { FrameBufferTextureFormat::Depth, ImageFilterType::Nearest }
        //     }
        // });
        
        m_FrameBuffer = FrameBuffer::CreateRef({
            .Width = 1920,
            .Height = 1080,
            .AttachmentsSpecs = {
                { .TextureFormat = FrameBufferTextureFormat::RGBA8, .FilterType = ImageFilterType::Nearest },
                { .TextureFormat = FrameBufferTextureFormat::RED_INTEGER, .FilterType = ImageFilterType::Nearest },
                { .TextureFormat = FrameBufferTextureFormat::Depth, .FilterType = ImageFilterType::Nearest }
            }
        });
        
        m_Cpp = Texture2D::CreateRef("assets/textures/cpp.png", { .Filter = Texture2D::FilterType::Nearest});

        m_ActiveScene = m_EditorContext.GetScene();
        m_SceneHierarchyPanel.SetScene(m_ActiveScene.get());
        
        auto commandLineArgs = Application::Get().GetCommandLineArgs();
        if (commandLineArgs.Count > 1)
        {
            std::string sceneFilePath = commandLineArgs[1];
            m_EditorContext.OpenScene(sceneFilePath);
        }
    }

    void EditorViewLayer::OnAttach()
    {
        Layer::OnAttach();
    }

    void EditorViewLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void EditorViewLayer::OnUpdate()
    {
        Layer::OnUpdate();
        
        if ((m_IsEditorViewportFocused || m_IsEditorViewportHovered) && !m_IsEditorViewportUsingGizmo)
        {
            m_EditorCamera.OnUpdate();
        }
        
        RenderEditorViewport();
    }

    void EditorViewLayer::OnImGuiRender()
    {
        Layer::OnImGuiRender();

        static bool dockSpaceOpened = true;
        static bool fullscreen = true;
        static bool keepWindowPadding = false;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (fullscreen)
        {
            // Fullscreen dockspace: practically the same as calling DockSpaceOverViewport();
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            window_flags |= ImGuiWindowFlags_NoBackground;
            window_flags |= ImGuiWindowFlags_MenuBar;
        }
        else
        {
            // Floating dockspace
            dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!keepWindowPadding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Window with a DockSpace", &dockSpaceOpened, window_flags);
        
        if (!keepWindowPadding)
            ImGui::PopStyleVar();

        if (fullscreen)
            ImGui::PopStyleVar(2);
        
        // Submit the DockSpace widget inside our window
        // - Note that the id here is different from the one used by DockSpaceOverViewport(), so docking state won't get transfered between "Basic" and "Advanced" demos.
        // - If we made the ShowExampleAppDockSpaceBasic() calculate its own ID and pass it to DockSpaceOverViewport() the ID could easily match.
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);

        // ------------ Menu Bar ------------
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::BeginDisabled(true);
                if (m_EditorContext.GetActiveSceneFilePath().empty())
                    ImGui::MenuItem("Current Scene: Untitled*");
                else
                    ImGui::MenuItem((std::format("Current Scene: {}", m_EditorContext.GetActiveSceneName()).c_str()));
                ImGui::EndDisabled();
                
                if (ImGui::MenuItem("New Scene", "Ctrl + N"))
                    m_EditorContext.NewScene();

                if (ImGui::MenuItem("Open Scene...", "Ctrl + O"))
                    m_EditorContext.OpenScene();

                if (ImGui::MenuItem("Save Scene", "Ctrl + S"))
                    m_EditorContext.SaveScene();

                if (ImGui::MenuItem("Save Scene As...", "Ctrl + Shift + S"))
                    m_EditorContext.SaveSceneAs();

                if (ImGui::MenuItem("Save & Exit"))
                {
                    if (m_EditorContext.SaveScene())
                        Application::Get().QuitApplication();
                    else
                        PKE_CORE_WARN("Failed to save scene before exiting.");
                }

                if (ImGui::MenuItem("Exit", "Alt + F4"))
                    Application::Get().QuitApplication();

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();

        if (ImGui::Begin("Editor Viewport Properties"))
        {
            // ImGui::Text("Editor Viewport Size: %.0f x %.0f", ImGui::GetContentRegionMax(), m_ViewportBounds.GetHeight());
            ImGui::Text("Editor Viewport Size: %.0f x %.0f", m_ViewportBounds.GetWidth(), m_ViewportBounds.GetHeight());
            ImGuiUtility::DrawBoundsControl("Editor Viewport Bounds", m_ViewportBounds);
            ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
            ImGui::Text("Editor Viewport Hovered: %s", m_IsEditorViewportHovered ? "Yes" : "No");
            ImGui::Text("Editor Viewport Focused: %s", m_IsEditorViewportFocused ? "Yes" : "No");
            if (ImGui::DragInt("Upscale Factor", &m_UpScaleFactor, 0.2f, 1, 40, "%i x"))
            {
                // m_FrameBuffer->Resize((uint32_t)(m_EditorViewportSize.x/(float)m_UpScaleFactor), (uint32_t)(m_EditorViewportSize.y/(float)m_UpScaleFactor));
            }
            
            ImGuiUtility::ShowDebugControls();
        }
        ImGui::End();

        ImGuiUtility::ShowApplicationInfoWindow();
        ImGuiUtility::ShowRendererStatistics(true);
        
        static bool inspector = true;
        if (ImGui::Begin("Editor Infos", &inspector, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Entity activeCamEntity = m_ActiveScene->GetMainCamera();
            if (activeCamEntity.IsValid())
                ImGui::TextColored({0.2f, 0.8f, 0.2f, 1.0f}, "Active Camera: %s", ENTITY_GET_NAME(activeCamEntity).c_str());
            
            ImGui::Text("Hovered Entity: %s", m_HoveredEntity.IsValid() ? ENTITY_GET_NAME(m_HoveredEntity).c_str() : "None");
        }
        ImGui::End();
        
        // This will be where the Viewport of Editor is rendered.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin("Editor Viewport"))
        {
            // ---------- Viewport State Update ------------
            m_IsEditorViewportFocused = ImGui::IsWindowFocused();
            m_IsEditorViewportHovered = ImGui::IsWindowHovered();
            
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            Bounds newViewportBounds = {
            { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y },
            { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y }
            };
            
            MAKE_DEBUG_CONTROL(bool, updateViewportBounds, true);
            
            if (updateViewportBounds && m_ViewportBounds.GetSize() != newViewportBounds.GetSize())
            {
                m_ViewportBounds = newViewportBounds;
                
                // m_UpScaledFrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
                // m_FrameBuffer->Resize((uint32_t)(viewportSize.x/(float)m_UpScaleFactor), (uint32_t)(viewportSize.y/(float)m_UpScaleFactor));
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportBounds.GetWidth(), (uint32_t)m_ViewportBounds.GetHeight());
                m_EditorCamera.SetViewportSize(m_ViewportBounds.GetWidth(), m_ViewportBounds.GetHeight());
            }
            // else
            // {
                // Rendering in the else branch helps decrease the flickering while resizing the viewport.
                // m_UpScaledFrameBuffer->ScaleFrom(*m_FrameBuffer);
            // }
            
            
            
            ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), m_ViewportBounds.GetSize(), {0, 1}, {1, 0});
            
            // ---------- GIZMOS ------------
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity.IsValid() && selectedEntity.HasComponent<TransformComponent>())
            {
                // Init
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                float windowWidth = ImGui::GetWindowWidth();
                float windowHeight = ImGui::GetWindowHeight();
                ImGuizmo::SetRect(m_ViewportBounds.Min.x, m_ViewportBounds.Min.y, windowWidth, windowHeight);

                // Camera
                glm::mat4 cameraProjection = m_EditorCamera.GetProjectionMatrix();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
                
                // Entity transform
                TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transformMatrix = transform;

                // Snapping
                bool snap = Input::IsKeyPressed(KeyCode::LeftCtrl);
                float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                if (m_GizmoOperation == ImGuizmo::OPERATION::ROTATE)
                    snapValue = 45.0f; // Snap to 45 degrees for rotation
                float snapValues[3] = {snapValue, snapValue, snapValue}; // Use the same snap value for all axes

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                     m_GizmoOperation, ImGuizmo::LOCAL,
                                     glm::value_ptr(transformMatrix), nullptr, snap ? snapValues : nullptr);
                
                m_IsEditorViewportUsingGizmo = ImGuizmo::IsUsing();
                if (m_IsEditorViewportUsingGizmo)
                {
                    glm::vec3 translation, rotation, scale;
                    
                    if (Math::DecomposeTransform(transformMatrix, translation, rotation, scale))
                    {
                        glm::vec3 originalRotation = transform.Rotation;
                        glm::vec3 deltaRotation = rotation - originalRotation;
                        
                        transform.Position = translation;
                        transform.Rotation += deltaRotation;
                        transform.Scale = scale;
                    }
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
        
        ImGui::End();
    }

    void EditorViewLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        
        if (m_IsEditorViewportFocused || m_IsEditorViewportHovered)
            m_EditorCamera.OnEvent(event);
        
        // Shortcuts
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e)
        {
            if (e.IsRepeat()) return false;

            bool control = Input::IsKeyPressed(KeyCode::LeftCtrl) || Input::IsKeyPressed(KeyCode::RightCtrl);
            bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);
            
            switch (e.GetKeyCode())
            {
                case KeyCode::N:
                    if (control)
                    {
                        m_EditorContext.NewScene();
                        return true;
                    }
                    break;
                    
                case KeyCode::O:
                    if (control)
                    {
                        m_EditorContext.OpenScene();
                        return true;
                    }
                    break;
                    
                case KeyCode::S:
                    if (control)
                    {
                        if (shift)
                            m_EditorContext.SaveSceneAs();
                        else
                            m_EditorContext.SaveScene();

                        return true;
                    }
                    break;
                    
                // ------- GIZMOS --------
                // case KeyCode::Q:
                //     m_GizmoOperation = -1;
                //     return true;
                case Controls::GIZMOS_TRANSLATE_KEY:
                    m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                    return true;
                case Controls::GIZMOS_ROTATE_KEY:
                    m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
                    return true;
                case Controls::GIZMOS_SCALE_KEY:
                    m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
                    return true;
                    
                // ------- CAMERA FOCUS --------
                case Controls::CAMERA_REFOCUS_KEY:
                {
                    // TODO: The selected entity should belong to the EditorContext instead of the SceneHierarchyPanel.
                    // TODO: Create event for when the selected entity changes.
                    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
                    if (selectedEntity.IsValid())
                    {
                        TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
                        m_EditorCamera.SetFocusPoint(transform.Position);
                        m_EditorCamera.SetDistance(10.0f);
                        return true;
                    }
                }
            }
            
            return false;
        });
        
        // Selection
        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e)
        {
            // --------- Mouse Picking ------------
            if (e.GetMouseButton() == Controls::MOUSE_PICK_BUTTON && m_IsEditorViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Controls::CAMERA_MODIFIER_KEY))
            {
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
                return true;
            }
            return false;
        });
    }

    void EditorViewLayer::RenderEditorViewport()
    {
        // Render in Frame Buffer
        m_FrameBuffer->Bind();
        
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();
        
        m_FrameBuffer->ClearAttachment(1, -1); // Clear the entity ID attachment to -1 (no entity)

        m_ActiveScene->OnEditorUpdate(m_EditorCamera);

        int viewportWidth = (int)m_ViewportBounds.GetWidth();
        int viewportHeight = (int)m_ViewportBounds.GetHeight();
        auto[mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds.Min.x;
        my -= m_ViewportBounds.Min.y; // Flip Y coordinate to match OpenGL's coordinate system
        my = (float)viewportHeight - my; // Flip Y coordinate to match OpenGL's coordinate system
        
        int mouseX = (int)mx;
        int mouseY = (int)my;


        bool mouseInViewport = mouseX >= 0 && mouseY >= 0 && mouseX < viewportWidth && mouseY < viewportHeight;
        if (mouseInViewport)
        {
            // Base resolution is 1920x1080, so we need to scale the mouse position to match the framebuffer size.
            glm::vec2 pixelPos = { 1920.0f*(float)mouseX/(float)viewportWidth, 1080.0f*(float)mouseY/(float)viewportHeight };
            
            int pixelData = m_FrameBuffer->ReadPixel(1, (int)pixelPos.x, (int)pixelPos.y);
            m_HoveredEntity = Entity((entt::entity)pixelData, m_ActiveScene.get());
        }
        
        m_FrameBuffer->Unbind();
    }
}
