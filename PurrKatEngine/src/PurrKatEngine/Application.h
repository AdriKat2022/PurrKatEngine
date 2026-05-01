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
        static Application& Get() { return *s_Instance; }
        
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e); // Will be run each time an event is triggered by the window.

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        Window& GetWindow() const { return *m_Window; }
        
    protected:
        virtual bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);
        virtual bool OnMouseMove(const CursorPosEvent& cursorPosEvent);
        virtual bool OnMouseScroll(const MouseScrollEvent& scrollEvent);

    private:
        static Application* s_Instance;
        
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
        
        LayerStack m_LayerStack;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
