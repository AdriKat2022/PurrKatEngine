#include "EditorViewLayer.h"

#include "PurrKatEngine/Serialization/SceneSerializer.h"
#include "PurrKatEngine/Utility/ArrayUtility.h"
#include "PurrKatEngine/Scene/Scene.h"

namespace PurrKatEngine
{
    EditorViewLayer::EditorViewLayer()
    {
        m_CameraController.EnableZoom = true;

        m_GrassSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Grass.png", {.Filter = Texture2D::FilterType::Nearest}));
        m_GrassSpriteSheet.SetSpriteSheetOptions({.CellCount = {11, 7}});

        m_UpScaledFrameBuffer = FrameBuffer::CreateRef({.Width = 1280, .Height = 720, .UpscalingFilterType = ImageFilterType::Nearest});
        m_FrameBuffer = FrameBuffer::CreateRef({.Width = 1920, .Height = 1080, .UpscalingFilterType = ImageFilterType::Nearest});
        m_Cpp = Texture2D::CreateRef("assets/textures/cpp.png", { .Filter = Texture2D::FilterType::Nearest});

        m_ActiveScene = m_EditorContext.GetScene();
        
        class CameraController : public ScriptableEntity
        {
        public:
            bool EnableMovement = true;
            bool EnableRotation = true;
            
            void OnStart() override
            {
                PKE_CORE_DEBUG("ON START!");
                TransformComponent& transform = m_Entity.GetComponent<TransformComponent>();
                SceneCamera& cam = GetComponent<CameraComponent>();
                cam.SetOrthographicSize(Random::Float(0.5f, 15.0f));
            }
            
            void OnUpdate() override
            {
                TransformComponent& transform = m_Entity.GetComponent<TransformComponent>();
                
                if (!HasComponent<CameraComponent>())
                    return;
                
                SceneCamera& cam = GetComponent<CameraComponent>();
                
                float camRotation = 0;
                
                if (EnableMovement)
                {
                    glm::vec3 camPos = transform.Position;
                    
                    auto input = Input::GetAxis2D(KeyCode::W, KeyCode::A, KeyCode::S, KeyCode::D);
        
                    camPos.x += (
                        cos(camRotation) * input.x
                        -sin(camRotation) * input.y
                    ) * (float)Time::deltaTime * cam.GetOrthographicSize();
        
                    camPos.y += (
                        cos(camRotation) * input.y +
                        sin(camRotation) * input.x
                    ) * (float)Time::deltaTime * cam.GetOrthographicSize();
                    
                    transform.Position = camPos;
                }

                if (EnableRotation)
                {
                    auto input = Input::GetAxis(KeyCode::Q, KeyCode::E);
                    camRotation += input * (float)Time::deltaTime;
                }
                
            }
        };
        
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
        
        if (m_IsEditorViewportFocused)
            m_CameraController.OnUpdate();

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
            ImGui::Text("Editor Viewport Size: %.0f x %.0f", m_LastEditorViewportSize.x, m_LastEditorViewportSize.y);
            ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
            ImGui::Text("Editor Viewport Receiving Events: %s", (!m_IsEditorViewportHovered || !m_IsEditorViewportFocused) ? "No" : "Yes");
            if (ImGui::DragInt("Upscale Factor", &m_UpScaleFactor, 0.2f, 1, 40, "%i x"))
            {
                m_FrameBuffer->Resize((uint32_t)(m_LastEditorViewportSize.x/(float)m_UpScaleFactor), (uint32_t)(m_LastEditorViewportSize.y/(float)m_UpScaleFactor));
            }
        }
        ImGui::End();

        ImGuiUtility::ShowApplicationInfoWindow();
        ImGuiUtility::ShowOrthographicCameraInfos(m_CameraController);
        ImGuiUtility::ShowRendererStatistics(true);
        
        static bool inspector = true;
        if (ImGui::Begin("Camera Switcher", &inspector, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Entity activeCamEntity = m_ActiveScene->GetMainCamera();
            
            static int activeCamera = ArrayUtility::IndexOf(activeCamEntity, m_CameraList);
            
            if (activeCamEntity.IsValid())
                ImGui::TextColored({0.2f, 0.8f, 0.2f, 1.0f}, "Active Camera: %s", ENTITY_GET_NAME(activeCamEntity).c_str());
            
            if (ImGui::Button("Switch Camera"))
            {
                activeCamera = (activeCamera + 1)%m_CameraList.size();
                auto& newActiveCamEntity = m_CameraList[activeCamera%m_CameraList.size()];
                m_ActiveScene->SetMainCamera(newActiveCamEntity);
            }
        }
        ImGui::End();
        
        // This will be where the Viewport of Editor is rendered.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin("Editor Viewport"))
        {
            m_IsEditorViewportFocused = ImGui::IsWindowFocused();
            m_IsEditorViewportHovered = ImGui::IsWindowHovered();
            // Application::Get().GetImGuiLayer().SetBlockEvents(!m_IsEditorViewportHovered || !m_IsEditorViewportFocused);
            contentSize = ImGui::GetContentRegionAvail();
            viewportSize = {contentSize.x, contentSize.y};
            if (viewportSize != m_LastEditorViewportSize)
            {
                m_LastEditorViewportSize = viewportSize;
                
                m_UpScaledFrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
                // m_FrameBuffer->Resize((uint32_t)(viewportSize.x/(float)m_UpScaleFactor), (uint32_t)(viewportSize.y/(float)m_UpScaleFactor));
                m_CameraController.SetAspectRatio(viewportSize.x/viewportSize.y);
                m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            }
            else
            {
                // Rendering in the else branch helps decrease the flickering while resizing the viewport.
                // m_UpScaledFrameBuffer->ScaleFrom(*m_FrameBuffer);
            }
            ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), *(ImVec2*)&m_LastEditorViewportSize, {0, 1}, {1, 0});
            
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
                Entity cameraEntity = m_ActiveScene->GetMainCamera();
                CameraComponent& mainCamera = cameraEntity.GetComponent<CameraComponent>();
                glm::mat4 cameraProjection = mainCamera.Camera.GetProjectionMatrix();
                glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransformMatrix());

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
                
                if (ImGuizmo::IsUsing())
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
        
        // Block window events because we already handle the viewport manually via ImGui.
        if (!event.IsInCategory(EventCategoryApplication) && m_IsEditorViewportHovered)
            m_CameraController.OnEvent(event);
        
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

        m_ActiveScene->OnUpdate();

        // Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
        // Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f});
        // Renderer2D::DrawQuad({1.0f, 1.0f}, {1.0f, 1.0f}, m_GrassSpriteSheet.GetSprite({0, 0}));
        // Renderer2D::DrawQuad({1.0f, 1.0f}, SET_WIDTH(m_Cpp, 1), m_Cpp);
        // Renderer2D::EndScene();

        m_FrameBuffer->Unbind();
    }
}
