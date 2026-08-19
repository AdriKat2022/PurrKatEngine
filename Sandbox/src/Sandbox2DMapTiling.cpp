#include "Sandbox2DMapTiling.h"

using namespace PurrKatEngine;

Sandbox2DMapTiling::Sandbox2DMapTiling()
{
    m_CameraController.EnableZoom = true;
}

void Sandbox2DMapTiling::OnAttach()
{
    Layer::OnAttach();
}

void Sandbox2DMapTiling::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2DMapTiling::OnUpdate()
{
    Layer::OnUpdate();
    m_CameraController.OnUpdate();
    
    RenderCommand::Clear();
    
    Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
    Renderer2D::DrawQuad({0, 0}, {1, 1});
    Renderer2D::EndScene();
}

void Sandbox2DMapTiling::OnImGuiRender()
{
    Layer::OnImGuiRender();
    ImGuiUtility::ShowOrthographicCameraInfos(m_CameraController);
}

void Sandbox2DMapTiling::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
    
    PKE_LOG_DEBUG("Event: {}", event.ToString());
}
