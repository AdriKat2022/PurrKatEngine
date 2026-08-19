#pragma once
#include "PurrKatEngine/Layers/Layer.h"
#include "PurrKatEngine/Renderer/OrthographicCameraController.h"
#include "PurrKatEngine/Renderer/SpriteSheet.h"

namespace PurrKatEngine
{
    class SpriteSheetLayerDebugging : public Layer
    {
    public:
        explicit SpriteSheetLayerDebugging(const Ref<Texture2D>& spriteSheetTexture);
        explicit SpriteSheetLayerDebugging(const std::string& path);
        explicit SpriteSheetLayerDebugging(SpriteSheet* spriteSheet);
        
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
        void OnImGuiRender() override;
        
    private:
        void CenterCameraOnSpriteSheet();
        
    private:
        OrthographicCameraController m_CameraController;
        SpriteSheet* m_SpriteSheet;
        std::vector<Tex2D> m_Sprites;
        
        glm::ivec2 m_SpriteSheetSpacing = {0, 0};
        float m_SpriteSheetZoom = 1;
        bool m_KeepCameraCentered = true;
    };
}
