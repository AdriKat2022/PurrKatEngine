#pragma once
#include "PurrKatEngine/Events/CursorPosEvent.h"
#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Events/MouseButtonPressedEvent.h"
#include "PurrKatEngine/Events/MouseButtonReleasedEvent.h"
#include "PurrKatEngine/Events/MouseScrollEvent.h"
#include "PurrKatEngine/Events/WindowResizeEvent.h"
#include "PurrKatEngine/Layers/Layer.h"

namespace PurrKatEngine
{
    class PKE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& e) override;

    private:
        void OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        void OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        void OnMouseMovedEvent(CursorPosEvent& e);
        void OnMouseScrollEvent(MouseScrollEvent& e);
        void OnKeyPressedEvent(KeyPressedEvent& e);
        void OnKeyReleasedEvent(KeyReleasedEvent& e);
        void OnWindowResize(WindowResizeEvent& e);
        
    private:
        float m_Time = 0.0f;
    };
}
