#include "EditorViewLayer.h"

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

                if (ImGui::MenuItem("Exit without saving", "Alt + F4"))
                    Application::Get().QuitApplication();

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        static ImVec2 contentSize = {};
        static glm::vec2 viewportSize = {};

        if (ImGui::Begin("Editor Viewport Properties"))
        {
            ImGui::Text("Editor Viewport Size: %.0f x %.0f", m_EditorViewportSize.x, m_EditorViewportSize.y);
            ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
            ImGui::Text("Editor Viewport Receiving Events: %s", (!m_IsEditorViewportHovered || !m_IsEditorViewportFocused) ? "No" : "Yes");
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
        if (ImGui::Begin("Camera Switcher", &inspector, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Entity activeCamEntity = m_ActiveScene->GetMainCamera();
            
            if (activeCamEntity.IsValid())
                ImGui::TextColored({0.2f, 0.8f, 0.2f, 1.0f}, "Active Camera: %s", ENTITY_GET_NAME(activeCamEntity).c_str());
            
            // static int activeCamera = ArrayUtility::IndexOf(activeCamEntity, m_CameraList);
            // if (ImGui::Button("Switch Camera"))
            // {
            //     activeCamera = (int)((activeCamera + 1)%m_CameraList.size());
            //     auto& newActiveCamEntity = m_CameraList[activeCamera%m_CameraList.size()];
            //     m_ActiveScene->SetMainCamera(newActiveCamEntity);
            // }
        }
        ImGui::End();
        
        // This will be where the Viewport of Editor is rendered.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin("Editor Viewport"))
        {
            // Viewport Size State
            m_IsEditorViewportFocused = ImGui::IsWindowFocused();
            m_IsEditorViewportHovered = ImGui::IsWindowHovered();
            contentSize = ImGui::GetContentRegionAvail();
            viewportSize = {contentSize.x, contentSize.y};
            if (viewportSize != m_EditorViewportSize)
            {
                m_EditorViewportSize = viewportSize;
                
                // m_UpScaledFrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
                // m_FrameBuffer->Resize((uint32_t)(viewportSize.x/(float)m_UpScaleFactor), (uint32_t)(viewportSize.y/(float)m_UpScaleFactor));
                m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
                m_EditorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
            }
            else
            {
                // Rendering in the else branch helps decrease the flickering while resizing the viewport.
                // m_UpScaledFrameBuffer->ScaleFrom(*m_FrameBuffer);
            }
            ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), *(ImVec2*)&m_EditorViewportSize, {0, 1}, {1, 0});
            
            auto viewportOffset = ImGui::GetCursorPos();
            auto windowSize = ImGui::GetWindowSize();
            ImVec2 minBound = ImGui::GetWindowPos();

            m_ViewportBounds[0] = {
                minBound.x + viewportOffset.x,
                minBound.y + viewportOffset.y
            };
            m_ViewportBounds[1] = {
                m_ViewportBounds[0].x + windowSize.x,
                m_ViewportBounds[0].y + windowSize.y
            };
            
            // PKE_CORE_INFO("Editor Viewport Size: {:.2f}-{:.2f} x {:.2f}-{:.2f}", windowSize.x, m_EditorViewportSize.x, windowSize.y, m_EditorViewportSize.y);
            // PKE_CORE_INFO("Editor Viewport Bounds: Min({:.2f}, {:.2f}), Max({:.2f}, {:.2f})", m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x, m_ViewportBounds[1].y);
            
            
            // ---------- GIZMOS ------------
            Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
            if (selectedEntity.IsValid() && selectedEntity.HasComponent<TransformComponent>())
            {
                // Init
                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                float windowWidth = ImGui::GetWindowWidth();
                float windowHeight = ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                // Camera
                // Entity cameraEntity = m_ActiveScene->GetMainCamera();
                // CameraComponent& mainCamera = cameraEntity.GetComponent<CameraComponent>();
                // glm::mat4 cameraProjection = mainCamera.Camera.GetProjectionMatrix();
                // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransformMatrix());
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
                
                m_IsEditorViewportUsingGizmo = false;
                if (ImGuizmo::IsUsing())
                {
                    m_IsEditorViewportUsingGizmo = true;
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
                case KeyCode::W:
                    m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                    return true;
                case KeyCode::E:
                    m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
                    return true;
                case KeyCode::R:
                    m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
                    return true;
                    
                // ------- CAMERA FOCUS --------
                case KeyCode::F:
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
    }

    void EditorViewLayer::RenderEditorViewport() const
    {
        // Render in Frame Buffer
        m_FrameBuffer->Bind();
        
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();
        
        m_FrameBuffer->ClearAttachment(1, -1); // Clear the entity ID attachment to -1 (no entity)

        m_ActiveScene->OnEditorUpdate(m_EditorCamera);

        auto[mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my = m_ViewportBounds[0].y - my; // Flip Y coordinate to match OpenGL's coordinate system
        
        int mouseX = (int)mx;
        int mouseY = (int)my;
        
        bool mouseInViewport = mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_EditorViewportSize.x && mouseY < (int)m_EditorViewportSize.y;
        if (mouseInViewport)
        {
            glm::vec2 pixelPos = { (float)1920*mouseX/m_EditorViewportSize.x, (float)1080*mouseY/m_EditorViewportSize.y };
            // glm::vec2 pixelPos = { (float)1920*mouseX/m_EditorViewportSize.x, (float)1080*mouseY/m_EditorViewportSize.y };
            
            int pixelData = m_FrameBuffer->ReadPixel(1, (int)pixelPos.x, (int)pixelPos.y);
            PKE_CORE_DEBUG("Mouse Position in Editor Viewport: ({}, {}) (pixel: {})", mouseX, mouseY, pixelData);
            // PKE_CORE_DEBUG("READ: {}", pixelData);
        }
        // PKE_CORE_DEBUG("Mouse Position in Editor Viewport: ({}, {})", mouseX, mouseY);
        
        
        m_FrameBuffer->Unbind();
    }
}
