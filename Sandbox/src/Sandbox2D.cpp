#include "Sandbox2D.h"

using namespace PKE;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(16/9.f, 1.f, true)
{}

void Sandbox2D::OnAttach()
{
    Layer::OnAttach();
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate()
{
    Layer::OnUpdate();
    m_CameraController.OnUpdate();
    
    RenderCommand::SetClearColor(m_BackgroundColor);
    RenderCommand::Clear();
    
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_SquareColor);
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();
    
    static bool showSettings = true;
    if (ImGui::Begin("Settings", &showSettings, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    }
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
}
