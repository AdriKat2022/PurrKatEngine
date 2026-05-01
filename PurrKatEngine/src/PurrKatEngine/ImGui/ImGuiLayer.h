#pragma once
#include "PurrKatEngine/Events/CursorEnterEvent.h"
#include "PurrKatEngine/Events/MouseMovedEvent.h"
#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Events/KeyTypedEvent.h"
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
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& e) override;

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrollEvent(MouseScrollEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);
        
    private:
        float m_Time = 0.0f;
    };
}
