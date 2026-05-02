#pragma once

#include "Core.h"
#include "Events/ApplicationEvents.h"
#include "ImGui/ImGuiLayer.h"
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

    private:
        static Application* s_Instance;
        
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_IsRunning = true;
        
        LayerStack m_LayerStack;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
