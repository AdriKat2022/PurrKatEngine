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
    PKE::OrthographicCameraController m_CameraController;
    PKE::SpriteSheet m_DirtSpriteSheet;
    PKE::SpriteSheet m_GrassSpriteSheet;
    
    std::unordered_map<char, PKE::Tex2D> m_TileMap;
};
