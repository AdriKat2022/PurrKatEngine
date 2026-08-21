#pragma once
#include "PurrKatEngine.h"

class EditorViewLayer : public PKE::Layer
{
public:
    EditorViewLayer();
    
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnImGuiRender() override;
    void OnEvent(PurrKatEngine::Event& event) override;

private:
    glm::vec4 m_BackgroundColor{0.1f, 0.1f, 0.1f, 1.0f};
    glm::vec2 m_LastEditorViewportSize;
    
    PKE::Ref<PKE::FrameBuffer> m_FrameBuffer;
    
    PKE::OrthographicCameraController m_CameraController;
    PKE::SpriteSheet m_DirtSpriteSheet;
    PKE::SpriteSheet m_GrassSpriteSheet;
};
