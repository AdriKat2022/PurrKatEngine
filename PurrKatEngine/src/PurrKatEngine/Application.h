#pragma once

#include "Core.h"
#include "Events/ApplicationEvents.h"
#include "ImGui/ImGuiLayer.h"
#include "Inputs/Time.h"
#include "Layers/LayerStack.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
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

        // OrthographicCamera& GetMainCamera() { return m_Camera; }
        Window& GetWindow() const { return *m_Window; }
        
    protected:
        virtual bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);

    private:
        static Application* s_Instance;
        
        bool m_IsRunning = true;
        
        Scope<Window> m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        TimeManagerLayer* m_TimeManagerLayer;
        
        // OrthographicCamera m_Camera;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
