#include "Sandbox2DLightTestScene.h"
#include <glm/gtc/type_ptr.hpp>
#include "PurrKatEngine/Renderer/SpriteSheet.h"

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
    
    m_ParticleSystem.SetMaxParticleCount(1000);
    
    m_RazowskiTexture = CreateRef(Texture2D::Create("assets/textures/razowski.png"));
    m_LoveTexture = CreateRef(Texture2D::Create("assets/textures/love.png", { .Filter = Texture2D::FilterType::Nearest }));
    m_CppTexture = CreateRef(Texture2D::Create("assets/textures/cpp.png"));
    m_FreddyTexture = CreateRef(Texture2D::Create("assets/textures/freddy.png"));
    m_BackgroundTexture = CreateRef(Texture2D::Create("assets/textures/hollowKnightBg.png", { .Filter = Texture2D::FilterType::Nearest }));
    m_MobTexture = CreateRef(Texture2D::Create("assets/textures/mob.png"));
    m_CreeperTexture = CreateRef(Texture2D::Create("assets/textures/creeper.png"));
    
    m_SpriteSheet = CreateRef(Texture2D::Create("assets/textures/spriteSheet.png", { .Filter = Texture2D::FilterType::Nearest }));
    
    m_SpriteSheetTest = CreateRef(new SpriteSheet(m_SpriteSheet, { .CellSize = {70, 70}, .Padding = {2, 2} }));
    
    m_Exclamation = m_SpriteSheetTest->GetSprite({0, 2});
    m_Cross = m_SpriteSheetTest->GetSprite({0, 11});
    
    m_Particle.LifeTime = 1.0f;
    m_Particle.Velocity = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 1.0f, 0.1f };
    m_Particle.ColorBegin = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_Particle.ColorEnd = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_Particle.SizeVariation = 0.5f;
    m_Particle.SizeBegin = 0.7f;
    m_Particle.SizeEnd = 0;
    
    m_FrameBuffer = FrameBuffer::CreateRef({
        .Width = Application::Get().GetWindow().GetWidth()/m_Upscaling,
        .Height = Application::Get().GetWindow().GetHeight()/m_Upscaling,
        .AttachmentsSpecs = {
            { FrameBufferTextureFormat::RGBA8, ImageFilterType::Nearest },
            { FrameBufferTextureFormat::Depth, ImageFilterType::Nearest }
        }
    });
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
    
    m_FrameBuffer->Bind();
    LightSource2D mouseLightSource;
    
    {
        PROFILE_SCOPE("Pre-Rendering");
        
        RenderCommand::SetClearColor(m_BackgroundColor);
        RenderCommand::Clear();
    
        glm::vec2 mousePosition = Input::GetMousePosition();
        
        // Dynamic light following mouse
        mouseLightSource = {
            .Position = glm::vec2(m_CameraController.GetCamera().ScreenToWorldPosition(mousePosition)),
            .Color = m_LightColor,
            .Radius = m_LightRadius,
            .Intensity = m_LightIntensity,
        };
    }
    
    PROFILE_SCOPE("Rendering");
    
    MAKE_DEBUG_CONTROL(float, rotation, 45);
    MAKE_DEBUG_CONTROL(float, width, 1);
    
    static bool litScene = false;
    ADD_DEBUG_CONTROL(litScene);
    
    Renderer2D::BeginScene(m_CameraController.GetCamera(), litScene);
    
    Renderer2D::AddLightSource({
        .Position = m_SquareTransform.GetPosition(),
        .Color = m_LightColor,
        .Radius = m_LightRadius,
        .Intensity = m_LightIntensity,
    });
    
    if (m_LightOn)
        Renderer2D::AddLightSource(mouseLightSource);
 
    Renderer2D::DrawQuad({ .Position = {0.0f, 0.0f, 0.5f}, .Size = {2,2} });
    Renderer2D::DrawQuad({ .Position = {-1.0f, 0.0f, 0.5f}, .Size = {2,2} });
    
    Renderer2D::DrawQuad({0.0f, 0.0f}, SET_WIDTH(m_BackgroundTexture, 20), m_BackgroundTexture);
    Renderer2D::DrawQuad({3.8f, -2.2f}, {1, 1}, m_MobTexture);
    Renderer2D::DrawQuad({-14.0f, 0}, SET_WIDTH(m_FreddyTexture, 1.5f), m_FreddyTexture);
    Renderer2D::DrawQuad({3.0f, 1.9f}, SET_WIDTH(m_CreeperTexture, 0.8f), m_CreeperTexture);
    Renderer2D::DrawQuad({-7.3f, 1.0f}, SET_WIDTH(m_CppTexture, 1.0f), m_CppTexture);
    Renderer2D::DrawRotatedQuad(m_SquareTransform.GetPosition(), SET_WIDTH(m_LoveTexture, width), glm::radians(rotation), m_LoveTexture);
    
    Renderer2D::DrawQuad({-7.3f, -0.5f}, SET_WIDTH(m_Exclamation, 1.0f), m_Exclamation);
    Renderer2D::DrawQuad({-7.3f, -2.0f}, SET_WIDTH(m_Cross, 1.0f), m_Cross);
    
    Renderer2D::EndScene();
    
    if (false)
    {
        MAKE_DEBUG_CONTROL(float, speed, 1);
        MAKE_DEBUG_CONTROL(int, count, 20);
        
        Renderer2D::BeginScene(m_CameraController.GetCamera(), false);
    
        static float elapsedTime = 0.0f;
        elapsedTime += (float)Time::deltaTime * speed;
    
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count; j++)
            {
                glm::vec2 position = {i, j};
                glm::vec2 displacement = { glm::sin(elapsedTime * 0.5f + (float)(i + j) * 0.5f) * 0.1f, glm::cos(elapsedTime * 0.5f + (float)(i + j) * 0.5f) * 0.1f };
                glm::vec4 color = ((i + j) % 2 == 0) ? glm::vec4(1, 0.5f, 1, 1) : glm::vec4(0, 0, 1, 1.0f);
                Renderer2D::DrawQuad(position + displacement, {1, 1}, color);
            }
        }
    
        Renderer2D::EndScene();
    }
    
    if (true)
    {
        PROFILE_SCOPE("ParticleSystem");
        
        MAKE_DEBUG_CONTROL(int, emission, 0);
        
        if (Input::IsMouseButtonPressed(PKE_BUTTON_MouseLeft))
        {
            auto mousePos = Input::GetMousePosition();
            glm::vec3 worldPos = m_CameraController.GetCamera().ScreenToWorldPosition(mousePos);
            
            m_Particle.Position = { worldPos.x, worldPos.y };
            for (int i = 0; i<emission; i++)
                m_ParticleSystem.Emit(m_Particle);
        }
        m_ParticleSystem.OnUpdate();
        m_ParticleSystem.OnRender(m_CameraController.GetCamera());
        
        ADD_DEBUG_CONTROL(m_Particle.LifeTime);
        ADD_DEBUG_CONTROL(m_Particle.VelocityVariation);
        ADD_DEBUG_CONTROL(m_Particle.ColorBegin);
        ADD_DEBUG_CONTROL(m_Particle.ColorEnd);
    }

    m_FrameBuffer->Unbind();
    
    RenderCommand::BlitFramebuffer(
        m_FrameBuffer->GetRendererID(),
        m_FrameBuffer->GetSpecifications().Width,
        m_FrameBuffer->GetSpecifications().Height,
        Application::Get().GetWindow().GetWidth(),
        Application::Get().GetWindow().GetHeight()
    );
}

void Sandbox2DLightTestScene::OnImGuiRender()
{
    Layer::OnImGuiRender();
    
    PROFILE_FUNCTION();
    
    ImGuiUtility::ShowApplicationInfoWindow();
    
    static bool infos = true;
    if (ImGui::Begin("Infos", &infos, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGuiUtility::ShowDisplayMouseAndWorldPosition(&m_CameraController.GetCamera());
        ImGui::Separator();
        ImGuiUtility::ShowRendererStatistics(false);
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
        
        // ImGuiUtility::SliderInt("Max Particle Count", &m_ParticleSystem, &ParticleSystem::GetMaxParticleCount, &ParticleSystem::SetMaxParticleCount, 0, 5000);
        if (ImGui::DragInt("Upscaling", &m_Upscaling, 0.5f, 1, 40, "%i x"))
            m_FrameBuffer->Resize(Application::Get().GetWindow().GetWidth()/m_Upscaling, Application::Get().GetWindow().GetHeight()/m_Upscaling);
        
        static constexpr std::array<const char*, 3> aspectRatioOptions = {"None", "Match Width", "Match Height"};
        ImGuiUtility::EnumCombo("Camera Auto Adjust Aspect Ratio", m_CameraController.AspectRatioAdjustment, aspectRatioOptions);
        
        ImGuiUtility::ShowDebugControls();
        
        ImGuiUtility::ShowWatchedValues();
    }
    ImGui::End();
    
    static bool profiling = false;
    if (ImGui::Begin("Profiling", &profiling, ImGuiWindowFlags_AlwaysAutoResize))
    {
        PROFILE_IMGUI_DISPLAY();
    }
    ImGui::End();
}

void Sandbox2DLightTestScene::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    m_CameraController.OnEvent(event);
    m_InputMoveSquareController.OnEvent(event);
}
