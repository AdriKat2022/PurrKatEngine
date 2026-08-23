#pragma once
#include "PurrKatEngine.h"

namespace PurrKatEngine
{
    class EditorViewLayer : public Layer
    {
        public:
        EditorViewLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

        private:
        glm::vec4 m_BackgroundColor{0.1f, 0.1f, 0.1f, 1.0f};
        glm::vec2 m_LastEditorViewportSize;

        Ref<FrameBuffer> m_FrameBuffer;

        OrthographicCameraController m_CameraController;
        SpriteSheet m_DirtSpriteSheet;
        SpriteSheet m_GrassSpriteSheet;
    };
}
