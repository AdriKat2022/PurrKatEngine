#pragma once
#include "PurrKatEngine.h"

class Sandbox2D : public PKE::Layer
{
public:
    Sandbox2D();
    
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnImGuiRender() override;
    void OnEvent(PKE::Event& event) override;
    
private:
    PKE::OrthographicCameraController m_CameraController;
    
    PKE::Standard2DInputController m_InputMoveSquareController;
    PKE::Transform m_SquareTransform;
    
    // Lighting Settings
    float m_LightAmbiance = 0.3f;
    float m_LightIntensity = 1.2f;
    float m_LightRadius = 2.0f;
    float m_ElapsedTime = 0.0f;
    
    glm::vec4 m_BackgroundColor = { 0.05f, 0.05f, 0.12f, 1.0f };
    glm::vec3 m_LightColor = { 1.0f, 0.95f, 0.8f };
    
    // Textures
    PKE::Scope<PKE::Texture2D> m_RazowskiTexture;
    PKE::Scope<PKE::Texture2D> m_LoveTexture;
    PKE::Scope<PKE::Texture2D> m_CppTexture;
    PKE::Scope<PKE::Texture2D> m_FreddyTexture;

    PKE::ShaderLibrary m_ShaderLibrary;
};
