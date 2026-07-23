#include "Sandbox2DLightTestScene.h"
#include <glm/gtc/type_ptr.hpp>

using namespace PKE;

Sandbox2DLightTestScene::Sandbox2DLightTestScene() :
    Layer("Sandbox2D"),
    m_CameraController(16/9.f, 0.5f, true),
    m_InputMoveSquareController([this](glm::vec2 input)
    {
        m_SquareTransform.Move({input.x, input.y, 0.0f});
    }, KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow)
    
{
    m_InputMoveSquareController.SetSpeed(0.1f);
    
    m_RazowskiTexture = ToRef(Texture2D::Create("assets/textures/razowski.png"));
    m_LoveTexture = ToRef(Texture2D::Create("assets/textures/love.png"));
    m_CppTexture = ToRef(Texture2D::Create("assets/textures/cpp.png"));
    m_FreddyTexture = ToRef(Texture2D::Create("assets/textures/freddy.png"));
    m_BackgroundTexture = ToRef(Texture2D::Create("assets/textures/hollowKnightBg.png"));
    m_MobTexture = ToRef(Texture2D::Create("assets/textures/mob.png"));
    m_CreeperTexture = ToRef(Texture2D::Create("assets/textures/creeper.png"));
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
    
    Renderer2D::BeginScene(m_CameraController.GetCamera(), true);
    
    
    // Renderer2D::AddLightSource({
    //     .Position = m_SquareTransform.GetPosition(),
    //     .Color = m_LightColor,
    //     .Radius = m_LightRadius,
    //     .Intensity = m_LightIntensity,
    // });
    
    MAKE_DEBUG_CONTROL(float, rotation, 45);
    MAKE_DEBUG_CONTROL(float, width, 50);
    MAKE_DEBUG_CONTROL(float, speed, 1);
    MAKE_DEBUG_CONTROL(int, count, 20);
    
    Renderer2D::DrawLitQuad({-0.5f, -0.5f, 0.0f}, {1, 1});
    Renderer2D::DrawLitQuad(m_SquareTransform.GetPosition(), {1, 1});
    
    Renderer2D::EndScene();
    Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
    
    if (m_LightOn) Renderer2D::AddLightSource(lightSource);
    
    // Renderer2D::DrawRotatedQuad({0, 0, -0.4f}, SET_WIDTH(m_BackgroundTexture, width), rotation, m_BackgroundTexture, {1, 1}, {1, 1, 1, 1});
    // Renderer2D::DrawQuad({0, 0, -0.4f}, SET_WIDTH(m_BackgroundTexture, 50), m_BackgroundTexture, {1, 1});
    // Renderer2D::DrawQuad({0, 3, 0}, {1, 1}, {1,1,1,1});
    // Renderer2D::DrawQuad(m_SquareTransform.GetPosition(), {1, 1}, {0.5f, 0.5f, 0.5f, 1.0f});
    
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            static float elapsedTime = 0.0f;
            elapsedTime += (float)Time::deltaTime/100 * speed;
            glm::vec3 position = {i, j, 0};
            glm::vec3 displacement = glm::vec3(glm::sin(elapsedTime * 0.5f + (float)(i + j) * 0.5f) * 0.1f, glm::cos(elapsedTime * 0.5f + (float)(i + j) * 0.5f) * 0.1f, 0);
            glm::vec4 color = ((i + j) % 2 == 0) ? glm::vec4(1, 0.5f, 1, 1) : glm::vec4(0, 0, 1, 1.0f);
            Renderer2D::DrawQuad(position + displacement, {1, 1}, color);
        }
    }
    
    Renderer2D::DrawLitQuad({0.0f, 0.0f, -0.5f}, SET_WIDTH(m_BackgroundTexture, 20), m_BackgroundTexture);
    Renderer2D::DrawLitQuad({3.8f, -2.2f}, m_SquareTransform.GetScale(), m_MobTexture);
    Renderer2D::DrawLitQuad({-14.0f, 0}, SET_WIDTH(m_FreddyTexture, 1.5f), m_FreddyTexture);
    Renderer2D::DrawLitQuad({3.0f, 1.9f}, SET_WIDTH(m_CreeperTexture, 0.8f), m_CreeperTexture);
    Renderer2D::DrawLitQuad({-7.3f, 1.0f}, SET_WIDTH(m_CppTexture, 1.0f), m_CppTexture);
    Renderer2D::DrawLitQuad(m_SquareTransform.GetPosition(), SET_WIDTH(m_LoveTexture, width), m_LoveTexture);

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
        ImGui::Separator();
        auto stats = Renderer2D::GetStatistics();
        ImGui::Text("Draw Calls: %u", stats.DrawCalls);
        ImGui::Text("Quad Count: %u", stats.QuadCount);
        ImGui::Text("Indices: %u", stats.GetIndexCount());
        ImGui::Text("Vertices: %u", stats.GetVertexCount());
        Renderer2D::EndFrameStatistics();
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
        
        ImGui::Separator();
        
        ImGuiUtility::ShowDebugControls();
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
