#include "Sandbox2D.h"
#include <glm/gtc/type_ptr.hpp>

using namespace PKE;

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"),
    m_CameraController(16/9.f, 1.f, true)
{
    m_RazowskiTexture = ToScope(Texture2D::Create("assets/textures/razowski.png"));
    m_LoveTexture = ToScope(Texture2D::Create("assets/textures/love.png"));
    m_CppTexture = ToScope(Texture2D::Create("assets/textures/cpp.png"));
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
    
    m_ElapsedTime += Time::deltaTime;
    
    RenderCommand::SetClearColor(m_BackgroundColor);
    RenderCommand::Clear();
    
    glm::vec2 mousePosition = Input::GetMousePosition();
    
    // Dynamic light following mouse
    LightSource2D lightSource =
        {
            .Position = glm::vec2(m_CameraController.GetCamera().ScreenToWorldPosition(mousePosition)),
            .Color = m_LightColor,
            .Radius = m_LightRadius,
            .Intensity = m_LightIntensity,
        };
    
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    
    Renderer2D::AddLightSource(lightSource);
    
    // ========== BACKGROUND LAYER ==========
    // Animated background grid pattern
    float gridSize = 0.25f;
    float gridSpacing = 0.3f;
    int gridCount = 14;
    for (int y = -gridCount/2; y < gridCount/2; y++)
    {
        for (int x = -gridCount/2; x < gridCount/2; x++)
        {
            float distFromCenter = glm::length(glm::vec2(x, y));
            float pulse = glm::sin(m_ElapsedTime * 1.5f + distFromCenter * 0.5f) * 0.5f + 0.5f;
            float brightness = 0.15f + pulse * 0.1f;
            glm::vec3 pos = glm::vec3(x * gridSpacing, y * gridSpacing, -1.0f);
            glm::vec4 color = glm::vec4(0.3f, 0.4f, 0.6f, 1.0f) * brightness;
            Renderer2D::DrawLitQuad(pos, glm::vec2(gridSize), color, m_LightAmbiance * 1.5f);
        }
    }
    
    // ========== BACK LAYER OBJECTS ==========
    // Left back pillar
    float leftBobbing = glm::sin(m_ElapsedTime * 0.7f) * 0.15f;
    Renderer2D::DrawLitQuad({-2.2f, -0.5f + leftBobbing, -0.5f}, glm::vec2(0.4f, 1.2f), glm::vec4(0.4f, 0.3f, 0.8f, 1.0f), m_LightAmbiance);
    
    // Right back pillar
    float rightBobbing = glm::sin(m_ElapsedTime * 0.7f + 1.57f) * 0.15f;
    Renderer2D::DrawLitQuad({2.2f, -0.5f + rightBobbing, -0.5f}, glm::vec2(0.4f, 1.2f), glm::vec4(0.8f, 0.3f, 0.4f, 1.0f), m_LightAmbiance);
    
    // ========== CENTER SHOWCASE ==========
    // Main centerpiece - rotating
    float rotation = glm::sin(m_ElapsedTime * 0.5f) * 0.1f;
    Renderer2D::DrawLitQuad({0.0f, 0.2f, 0.0f}, glm::vec2(1.2f), m_RazowskiTexture.get(), glm::vec4(1.0f), 0.8f);
    
    // Top left accent - bobbing and rotating
    float topLeftBobbing = glm::sin(m_ElapsedTime * 1.2f) * 0.2f;
    float topLeftRotation = m_ElapsedTime * 1.5f;
    Renderer2D::DrawLitQuad({-1.2f, 1.0f + topLeftBobbing, 0.2f}, glm::vec2(0.5f), m_LoveTexture.get(), glm::vec4(1.0f, 0.8f, 0.3f, 1.0f), 0.7f);
    
    // Top right accent - bobbing opposite phase
    float topRightBobbing = glm::sin(m_ElapsedTime * 1.2f + 3.14f) * 0.2f;
    Renderer2D::DrawLitQuad({1.2f, 1.0f + topRightBobbing, 0.2f}, glm::vec2(0.5f), m_CppTexture.get(), glm::vec4(0.3f, 1.0f, 0.8f, 1.0f), 0.7f);
    
    // Bottom left detail
    float bottomLeftBobbing = glm::sin(m_ElapsedTime * 0.9f + 1.57f) * 0.1f;
    Renderer2D::DrawLitQuad({-1.5f, -1.0f + bottomLeftBobbing, -0.3f}, glm::vec2(0.35f), glm::vec4(1.0f, 0.5f, 0.5f, 1.0f), m_LightAmbiance);
    
    // Bottom right detail
    float bottomRightBobbing = glm::sin(m_ElapsedTime * 0.9f) * 0.1f;
    Renderer2D::DrawLitQuad({1.5f, -1.0f + bottomRightBobbing, -0.3f}, glm::vec2(0.35f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), m_LightAmbiance);
    
    // ========== FRONT LAYER GLOW ==========
    // Subtle foreground glow elements
    glm::vec4 glowColor = glm::vec4(0.7f, 0.8f, 1.0f, 0.5f);
    Renderer2D::DrawLitQuad({-3.5f, 0.5f, 0.8f}, glm::vec2(0.3f, 0.6f), glowColor, m_LightAmbiance * 0.5f);
    Renderer2D::DrawLitQuad({3.5f, 0.5f, 0.8f}, glm::vec2(0.3f, 0.6f), glowColor, m_LightAmbiance * 0.5f);
    Renderer2D::DrawLitQuad({0.0f, -2.0f, 0.5f}, glm::vec2(1.0f, 0.2f), glm::vec4(1.0f, 0.9f, 0.7f, 0.3f), m_LightAmbiance * 0.3f);
    
    Renderer2D::ClearLightSources();
    
    Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();
    
    static bool showSettings = true;
    if (ImGui::Begin("Lighting Showcase Settings", &showSettings, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Move your mouse to control the light position");
        ImGui::Separator();
        
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
        ImGui::DragFloat("Light Radius", &m_LightRadius, 0.01f, 0.1f, 5.0f);
        ImGui::DragFloat("Light Intensity", &m_LightIntensity, 0.01f, 0.0f, 3.0f);
        ImGui::DragFloat("Ambient Light", &m_LightAmbiance, 0.01f, 0.0f, 1.0f);
        ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
        
        ImGui::Separator();
        ImGui::Text("This scene showcases:");
        ImGui::BulletText("Dynamic mouse-following light");
        ImGui::BulletText("Animated objects with bobbing and rotation");
        ImGui::BulletText("Layered depth composition");
        ImGui::BulletText("Real-time lighting calculations");
    }
    ImGui::End();
}

void Sandbox2D::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
}
