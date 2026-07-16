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
    glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };

    PKE::ShaderLibrary m_ShaderLibrary;
    PKE::OrthographicCameraController m_CameraController;
};
