#pragma once
#include "PurrKatEngine.h"
#include "EditorPanels/SceneHierarchyPanel.h"

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
        void RenderEditorViewport();
        
    private:
        glm::vec4 m_BackgroundColor{0.1f, 0.1f, 0.1f, 1.0f};
        glm::vec2 m_LastEditorViewportSize;
        bool m_IsEditorViewportFocused = false;
        bool m_IsEditorViewportHovered = false;

        int m_UpScaleFactor = 1;
        
        Ref<FrameBuffer> m_FrameBuffer;
        Ref<FrameBuffer> m_UpScaledFrameBuffer;

        OrthographicCameraController m_CameraController;
        SpriteSheet m_DirtSpriteSheet;
        SpriteSheet m_GrassSpriteSheet;
        Ref<const Texture2D> m_Cpp;
        
        Scene m_ActiveScene;
        std::vector<Entity> m_CameraList;
        
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}
