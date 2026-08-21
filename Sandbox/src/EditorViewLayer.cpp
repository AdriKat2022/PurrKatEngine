#include "EditorViewLayer.h"

using namespace PurrKatEngine;

EditorViewLayer::EditorViewLayer()
{
    m_CameraController.EnableZoom = true;
    
    m_GrassSpriteSheet.SetTexture(Texture2D::CreateRef("assets/textures/TileSets/Grass.png", { .Filter = Texture2D::FilterType::Nearest }));
    m_GrassSpriteSheet.SetSpriteSheetOptions({ .CellCount = {11, 7} });
    
    m_FrameBuffer = FrameBuffer::CreateRef({.Width = 1280, .Height = 720});
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
    m_CameraController.OnUpdate();
    
    // Render in Frame Buffer
    m_FrameBuffer->Bind();
    
    RenderCommand::SetClearColor(m_BackgroundColor);
    RenderCommand::Clear();
    
    Renderer2D::BeginScene(m_CameraController.GetCamera(), false);

    Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f });
    
    Renderer2D::EndScene();
    
    m_FrameBuffer->Unbind();
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
    
    // This will be where the Viewport of Editor is rendered.
    if (ImGui::Begin("Editor Viewport"))
    {
        ImVec2 contentSize = ImGui::GetContentRegionAvail();
        glm::vec2 viewportSize = {contentSize.x, contentSize.y};
        if (viewportSize != m_LastEditorViewportSize)
        {
            m_LastEditorViewportSize = viewportSize;
            m_FrameBuffer->GetSpecifications().Width = (uint32_t)viewportSize.x;
            m_FrameBuffer->GetSpecifications().Height = (uint32_t)viewportSize.y;
            m_FrameBuffer->Invalidate();
        }

        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image(textureID, contentSize);
    }
    
    ImGui::End();
    
    if (ImGui::Begin("Editor Viewport Properties"))
    {
        ImGui::Text("Viewport Size: %.0f x %.0f", m_LastEditorViewportSize.x, m_LastEditorViewportSize.y);
        ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
    }
    ImGui::End();
    
    ImGuiUtility::ShowApplicationInfoWindow();
    ImGuiUtility::ShowOrthographicCameraInfos(m_CameraController);
    
    ImGui::End();
}

void EditorViewLayer::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
}
