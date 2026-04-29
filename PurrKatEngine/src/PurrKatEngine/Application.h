#pragma once

#include "Core.h"
#include "Events/CursorPosEvent.h"
#include "Events/MouseScrollEvent.h"
#include "Events/WindowCloseEvent.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    class PKE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e); // Will be run each time an event is triggered by the window.

        void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
        void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }
        
    protected:
        virtual bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);
        virtual bool OnMouseMove(const CursorPosEvent& cursorPosEvent);
        virtual bool OnMouseScroll(const MouseScrollEvent& scrollEvent);

    private:
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
        
        LayerStack m_LayerStack;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
