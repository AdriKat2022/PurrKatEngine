#include "EditorViewLayer.h"

namespace PurrKatEngine
{
    EditorViewLayer::EditorViewLayer()
    {
        m_CameraController.EnableZoom = true;

        m_GrassSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Grass.png", {.Filter = Texture2D::FilterType::Nearest}));
        m_GrassSpriteSheet.SetSpriteSheetOptions({.CellCount = {11, 7}});

        m_UpScaledFrameBuffer = FrameBuffer::CreateRef({.Width = 1280, .Height = 720, .UpscalingFilterType = FilterType::Nearest});
        m_FrameBuffer = FrameBuffer::CreateRef({.Width = 1920, .Height = 1080, .UpscalingFilterType = FilterType::Nearest});
        m_Cpp = Texture2D::CreateRef("assets/textures/cpp.png", { .Filter = Texture2D::FilterType::Nearest});

        m_SquareEntity = m_ActiveScene.CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4{1.0f, 0, 0, 1.0f});
        
        class CameraController : public ScriptableEntity
        {
        public:
            bool EnableMovement = true;
            bool EnableRotation = true;
            
            void OnStart() override
            {
                PKE_CORE_DEBUG("ON START!");
                glm::mat4& transform = GetComponent<TransformComponent>();
                SceneCamera& cam = GetComponent<CameraComponent>();
                cam.SetOrthographicSize(Random::Float(0.5f, 15.0f));
            }
            
            void OnUpdate() override
            {
                glm::mat4& transform = GetComponent<TransformComponent>();
                SceneCamera& cam = GetComponent<CameraComponent>();
                
                float camRotation = 0;
                
                if (EnableMovement)
                {
                    glm::vec2 camPos = { transform[3][0], transform[3][1] };
                    
                    auto input = Input::GetAxis2D(KeyCode::W, KeyCode::A, KeyCode::S, KeyCode::D);
        
                    camPos.x += (
                        cos(camRotation) * input.x
                        -sin(camRotation) * input.y
                    ) * (float)Time::deltaTime * cam.GetOrthographicSize();
        
                    camPos.y += (
                        cos(camRotation) * input.y +
                        sin(camRotation) * input.x
                    ) * (float)Time::deltaTime * cam.GetOrthographicSize();
                    
                    transform[3][0] = camPos.x;
                    transform[3][1] = camPos.y;
                }

                if (EnableRotation)
                {
                    auto input = Input::GetAxis(KeyCode::Q, KeyCode::E);
                    camRotation += input * (float)Time::deltaTime;
                    // transform[3][3] = camRotation;
                }
                
            }
        };
        
        m_CameraEntity = m_ActiveScene.CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();
        m_CameraEntity.AddComponent<ScriptComponent>().Bind<CameraController>();
         
        m_CameraEntity2 = m_ActiveScene.CreateEntity("Camera2");
        m_CameraEntity2.AddComponent<CameraComponent>();
        m_CameraEntity2.AddComponent<ScriptComponent>().Bind<CameraController>();
        
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
                if (ImGui::MenuItem("Exit"))
                    Application::Get().QuitApplication();

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

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
        if (ImGui::Begin("Inspector", &inspector, ImGuiWindowFlags_AlwaysAutoResize))
        {
            std::string name = m_SquareEntity.GetComponent<TagComponent>();
            ImGui::TextColored({0.2f, 0.8f, 0.2f, 1.0f}, "Entity: %s", name.c_str());
            
            static bool state = false;
            if (ImGui::Button("Switch Camera"))
                state = !state;
            
            auto& activeCam = state ? m_CameraEntity : m_CameraEntity2;
            
            m_ActiveScene.SetMainCamera(activeCam);
            
            ImGui::DragFloat3("Camera 1", glm::value_ptr(activeCam.GetComponent<TransformComponent>().Transform[3]), 0.01f);
            
            const char* modes[] = {
                "None",
                "Match Width",
                "Match Height"
            };

            SceneCamera& camComponent = activeCam.GetComponent<CameraComponent>();
            
            float size = camComponent.GetOrthographicSize();
            if (ImGui::DragFloat("Camera Size", &size, 0.01f))
                camComponent.SetOrthographicSize(size);
            
            int mode = (int)camComponent.GetAspectRatioAdjustementMode();

            if (ImGui::Combo("Aspect Ratio", &mode, modes, IM_ARRAYSIZE(modes)))
                camComponent.SetAspectRatioAdjustementMode((SceneCamera::AspectRatioAdjustmentMode)mode);
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
                m_ActiveScene.OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            }
            else
            {
                // Rendering in the else branch helps decrease the flickering while resizing the viewport.
                // m_UpScaledFrameBuffer->ScaleFrom(*m_FrameBuffer);
            }
            ImGui::Image(m_FrameBuffer->GetColorAttachmentRendererID(), *(ImVec2*)&m_LastEditorViewportSize, {0, 1}, {1, 0});
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
    }

    void EditorViewLayer::RenderEditorViewport()
    {
        // Render in Frame Buffer
        m_FrameBuffer->Bind();
        
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();

        m_ActiveScene.OnUpdate();

        // Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
        // Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f});
        // Renderer2D::DrawQuad({1.0f, 1.0f}, {1.0f, 1.0f}, m_GrassSpriteSheet.GetSprite({0, 0}));
        // Renderer2D::DrawQuad({1.0f, 1.0f}, SET_WIDTH(m_Cpp, 1), m_Cpp);
        // Renderer2D::EndScene();

        m_FrameBuffer->Unbind();
    }
}
