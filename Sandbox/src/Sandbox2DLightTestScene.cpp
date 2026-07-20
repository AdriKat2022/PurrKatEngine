#include "Sandbox2DLightTestScene.h"
#include <glm/gtc/type_ptr.hpp>

using namespace PKE;

Sandbox2DLightTestScene::Sandbox2DLightTestScene() :
    Layer("Sandbox2D"),
    m_CameraController(16/9.f, 0.5f, true),
    m_InputMoveSquareController([this](glm::vec2 input)
    {
        m_SquareTransform.Move({input.x, input.y, 0});
    }, KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow)
    
{
    m_InputMoveSquareController.SetSpeed(0.05f);
    
    m_RazowskiTexture = ToScope(Texture2D::Create("assets/textures/razowski.png"));
    m_LoveTexture = ToScope(Texture2D::Create("assets/textures/love.png"));
    m_CppTexture = ToScope(Texture2D::Create("assets/textures/cpp.png"));
    m_FreddyTexture = ToScope(Texture2D::Create("assets/textures/freddy.png"));
    m_BackgroundTexture = ToScope(Texture2D::Create("assets/textures/hollowKnightBg.png"));
    m_MobTexture = ToScope(Texture2D::Create("assets/textures/mob.png"));
    m_CreeperTexture = ToScope(Texture2D::Create("assets/textures/creeper.png"));
    
    // Profiling
    
    m_ProfileResults.emplace_back("Update");
}

void Sandbox2DLightTestScene::OnAttach()
{
    Layer::OnAttach();
}

void Sandbox2DLightTestScene::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2DLightTestScene::OnUpdate()
{
    Layer::OnUpdate();
    
    {
        PROFILE_SCOPE("Controller");
        
        m_CameraController.OnUpdate();
        m_InputMoveSquareController.OnUpdate();
    }
    
    static bool active = true;
    if (Input::IsKeyPressed(KeyCode::Space))
    {
        if (active) m_LightOn = !m_LightOn;
        active = false;
    }
    else
    {
        active = true;
    }
    
    LightSource2D lightSource;
    
    {
        PROFILE_SCOPE("Pre-Rendering");
        
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();
    
        glm::vec2 mousePosition = Input::GetMousePosition();
    
        // Dynamic light following mouse
        lightSource = {
            .Position = glm::vec2(m_CameraController.GetCamera().ScreenToWorldPosition(mousePosition)),
            .Color = m_LightColor,
            .Radius = m_LightRadius,
            .Intensity = m_LightIntensity,
        };
    }
    
    PROFILE_SCOPE("Rendering");
    
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    
    if (m_LightOn) Renderer2D::AddLightSource(lightSource);
    
    // float leftBobbing = glm::sin(m_ElapsedTime * 0.7f) * 0.15f;
    glm::vec2 backgroundSize = glm::vec2(20.0f, 20.0f/m_BackgroundTexture->GetAspectRatio());
    Renderer2D::DrawLitQuad({0.0f, 0.0f, -0.6f}, backgroundSize, m_BackgroundTexture.get(), m_BackgroundColor, 0.4f + m_LightAmbiance / 1.6f);
    
    Renderer2D::DrawLitQuad({3.8f, -2.2f}, m_SquareTransform.GetScale(), m_MobTexture.get(), glm::vec4(1), 0);
    
    Renderer2D::DrawLitQuad({-14.0f, 0}, {1.5f, 1.5f/m_FreddyTexture->GetAspectRatio()}, m_FreddyTexture.get(), glm::vec4(1), 0);
    
    Renderer2D::DrawLitQuad({3.0f, 1.9f}, {0.8f, 0.8f/m_CreeperTexture->GetAspectRatio()}, m_CreeperTexture.get(), glm::vec4(1), 0);
    
    Renderer2D::DrawLitQuad({-7.3f, 1.0f}, {1.0f, 1.0f/m_CppTexture->GetAspectRatio()}, m_CppTexture.get(), glm::vec4(1), 0.5f + m_LightAmbiance/2.0f);

    Renderer2D::DrawLitQuad(m_SquareTransform.GetPosition(), {0.5f, 0.5f/m_LoveTexture->GetAspectRatio()}, m_LoveTexture.get(), glm::vec4(1), m_LightAmbiance - 0.5f);
    
    // ========== BACK LAYER OBJECTS ==========
    
    // Scope<Texture2D> checkerboardTexture = ToScope(Texture2D::Create(2, 2));
    // uint32_t checkerboardPixels[] = { 0xffffffff, 0x000000ff, 0x000000ff, 0xffffffff };
    // checkerboardTexture->SetData(checkerboardPixels, sizeof(checkerboardPixels));
    // Renderer2D::DrawLitQuad({0, 0, -0.5f}, glm::vec2(20.0f), checkerboardTexture.get(), m_BackgroundColor, m_LightAmbiance + 0.2f, {30, 30});
    
    // Left back pillar
    // float leftBobbing = glm::sin(m_ElapsedTime * 0.7f) * 0.15f;
    // Renderer2D::DrawLitQuad({-2.2f, -0.5f + leftBobbing, -0.4f}, glm::vec2(0.4f, 1.2f), glm::vec4(0.4f, 0.3f, 0.8f, 1.0f), m_LightAmbiance);
    
    // Right back pillar
    // float rightBobbing = glm::sin(m_ElapsedTime * 0.7f + 1.57f) * 0.15f;
    // Renderer2D::DrawLitQuad({2.2f, -0.5f + rightBobbing, -0.4f}, glm::vec2(0.4f, 1.2f), glm::vec4(0.8f, 0.3f, 0.4f, 1.0f), m_LightAmbiance);
    
    // Bottom left detail
    // float bottomLeftBobbing = glm::sin(m_ElapsedTime * 0.9f + 1.57f) * 0.1f;
    // Renderer2D::DrawLitQuad({-1.5f, -1.0f + bottomLeftBobbing, -0.1f}, glm::vec2(0.8f), glm::vec4(1.0f, 0.5f, 0.5f, 1.0f), m_LightAmbiance);
    
    // Bottom right detail
    // float bottomRightBobbing = glm::sin(m_ElapsedTime * 0.9f) * 0.1f;
    // Renderer2D::DrawLitQuad({1.5f, -1.0f + bottomRightBobbing, -0.1f}, glm::vec2(0.8f), glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), m_LightAmbiance);
    
    
    // ========== CENTER SHOWCASE ==========
    // Main centerpiece - rotating
    // float rotation = glm::sin(m_ElapsedTime * 0.5f) * 0.1f;
    // glm::vec2 size(1.0f, 1.0f*m_FreddyTexture->GetHeight() / m_FreddyTexture->GetWidth());
    // Renderer2D::DrawLitQuad(m_SquareTransform.GetPosition(), size, m_FreddyTexture.get(), glm::vec4(1.0f), 0.0f);
    
    // Top left accent - bobbing and rotating
    // float topLeftBobbing = glm::sin(m_ElapsedTime * 1.2f) * 0.2f;
    // float topLeftRotation = m_ElapsedTime * 1.5f;
    // Renderer2D::DrawLitQuad({-1.2f, 1.0f + topLeftBobbing, 0.2f}, glm::vec2(0.5f), m_LoveTexture.get(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.7f);

    // Top right accent - bobbing opposite phase
    // float topRightBobbing = glm::sin(m_ElapsedTime * 1.2f + 3.14f) * 0.2f;
    // Renderer2D::DrawLitQuad({1.2f, 1.0f + topRightBobbing, 0.2f}, glm::vec2(0.5f), m_CppTexture.get(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.7f);

    // ========== FRONT LAYER GLOW ==========
    // Subtle foreground glow elements
    // glm::vec4 glowColor = glm::vec4(0.7f, 0.8f, 1.0f, 0.5f);
    // Renderer2D::DrawLitQuad({-3.5f, 0.5f, 0.5f}, glm::vec2(0.3f, 0.6f), glowColor, m_LightAmbiance * 0.5f);
    // Renderer2D::DrawLitQuad({3.5f, 0.5f, 0.5f}, glm::vec2(0.3f, 0.6f), glowColor, m_LightAmbiance * 0.5f);
    // Renderer2D::DrawLitQuad({0.0f, -2.0f, 0.5f}, glm::vec2(1.0f, 0.2f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), m_LightAmbiance * 0.3f);
    
    Renderer2D::ClearLightSources();
    
    Renderer2D::EndScene();
}

void Sandbox2DLightTestScene::OnImGuiRender()
{
    Layer::OnImGuiRender();
    
    PROFILE_FUNCTION();
    
    ImGuiUtility::ApplicationInfoWindow(Application::Get());
    
    static bool infos = true;
    if (ImGui::Begin("Infos", &infos, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGuiUtility::DisplayMouseAndWorldPosition(&m_CameraController.GetCamera());
    }
    ImGui::End();
    
    static bool showSettings = true;
    if (ImGui::Begin("Lighting Showcase Settings", &showSettings, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Move your mouse to control the light position");
        ImGui::Separator();
        ImGui::Text("Light %s", m_LightOn ? "On" : "Off");
        ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
        ImGui::DragFloat("Light Radius", &m_LightRadius, 0.01f, 0.1f, 5.0f);
        ImGui::DragFloat("Light Intensity", &m_LightIntensity, 0.01f, 0.0f, 3.0f);
        ImGui::DragFloat("Ambient Light", &m_LightAmbiance, 0.01f, 0.0f, 1.0f);
        ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
        
        ImGui::Separator();
        ImGui::Text("This scene showcases:");
        ImGui::BulletText("Dynamic mouse-following light");
        ImGui::BulletText("Real-time lighting calculations");
    }
    ImGui::End();
    
    static bool profiling = true;
    if (ImGui::Begin("Profiling", &profiling, ImGuiWindowFlags_AlwaysAutoResize))
    {
        PROFILE_DISPLAY();
    }
    ImGui::End();
}

void Sandbox2DLightTestScene::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
    m_InputMoveSquareController.OnEvent(event);
}
