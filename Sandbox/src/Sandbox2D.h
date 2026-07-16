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
    
    glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    glm::vec4 m_SquareColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    PKE::Transform m_SquareTransform;
    PKE::Standard2DInputController m_InputMoveSquareController;
    PKE::Standard1DInputController m_InputSizeSquareController;

    PKE::ShaderLibrary m_ShaderLibrary;
};
