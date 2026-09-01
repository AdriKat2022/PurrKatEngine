#pragma once
#include "EditorContext.h"
#include "PurrKatEngine.h"
#include "EditorPanels/SceneHierarchyPanel.h"
#include "ImGuizmo.h"
#include "PurrKatEngine/Editor/EditorCamera.h"

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
        void RenderEditorViewport() const;
        
    private:
        EditorContext m_EditorContext;
        
        EditorCamera m_EditorCamera;
        
        // GIZMO
        ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        
        glm::vec4 m_BackgroundColor{0.1f, 0.1f, 0.1f, 1.0f};
        glm::vec2 m_ViewportBounds[2];
        glm::vec2 m_EditorViewportSize;
        bool m_IsEditorViewportFocused = false;
        bool m_IsEditorViewportHovered = false;
        bool m_IsEditorViewportUsingGizmo = false;

        int m_UpScaleFactor = 1;
        
        Ref<FrameBuffer> m_FrameBuffer;
        // Ref<FrameBuffer> m_UpScaledFrameBuffer;

        SpriteSheet m_DirtSpriteSheet;
        SpriteSheet m_GrassSpriteSheet;
        Ref<const Texture2D> m_Cpp;
        
        Ref<Scene> m_ActiveScene;
        std::vector<Entity> m_CameraList;
        
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}
