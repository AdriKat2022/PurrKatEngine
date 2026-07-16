#include "Sandbox2D.h"

using namespace PKE;

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(16/9.f, 1.f, true),
    m_InputMoveSquareController([this](glm::vec2 input)
    {
        m_SquareTransform.Move({input.x, input.y, 0});
    }, KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow),
    m_InputSizeSquareController([this](float input)
    {
        auto newScale = (float)m_SquareTransform.GetScale().x + input;
        m_SquareTransform.SetScale(glm::vec3(newScale));
    }, KeyCode::Keypad0, KeyCode::Keypad1)
{
    m_InputMoveSquareController.SetSpeed(0.05f);
    m_InputSizeSquareController.SetSpeed(0.05f);
    
    m_RazowskiTexture = ToScope(Texture2D::Create("assets/textures/razowski.png"));
}

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
    m_InputMoveSquareController.OnUpdate();
    m_InputSizeSquareController.OnUpdate();
    
    RenderCommand::SetClearColor(m_BackgroundColor);
    RenderCommand::Clear();
    
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad({-1.f, 0.5f}, m_SquareTransform.GetScale(), m_SquareColor + 0.2f);
    Renderer2D::DrawQuad({1.f, 0.5f, -0.2f}, m_SquareTransform.GetScale(), m_SquareColor - 0.2f);
    Renderer2D::DrawQuad(m_SquareTransform.GetPosition(), m_SquareTransform.GetScale(), m_RazowskiTexture.get(), m_SquareColor);
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
        
        if (ImGui::CollapsingHeader("Square Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto pos = m_SquareTransform.GetPosition();
            ImGui::Text("Square Position: %f, %f, %f", pos.x, pos.y, pos.z);
            ImGui::Text("Square Scale: %f", m_SquareTransform.GetScale().x);
        }
    }
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
    m_InputMoveSquareController.OnEvent(event);
    m_InputSizeSquareController.OnEvent(event);
}
