#pragma once
#include "PurrKatEngine.h"

class Sandbox2DMapTiling : public PKE::Layer
{
public:
    Sandbox2DMapTiling();
    
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnImGuiRender() override;
    void OnEvent(PurrKatEngine::Event& event) override;

private:
    PKE::OrthographicCameraController m_CameraController;
    PKE::SpriteSheet m_SpriteSheet;
};
