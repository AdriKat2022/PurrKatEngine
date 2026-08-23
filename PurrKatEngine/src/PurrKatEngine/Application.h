#pragma once

#include "Core.h"
#include "Events/ApplicationEvents.h"
#include "Inputs/Time.h"
#include "Layers/LayerStack.h"
#include "Renderer/VertexArray.h"
#include "Window/Window.h"

namespace PurrKatEngine
{
    class ImGuiLayer;
    
    #define PKE_VERSION 1.0
    #define PKE_VERSION_STR "1.0"
    constexpr const char* VERSION = "1.0";
    
    class PKE_API Application
    {
    public:        
        static Application& Get() { return *s_Instance; }
        
        Application(const std::string& appName = "PurrKatEngine v" PKE_VERSION_STR);
        virtual ~Application();

        void Run();
        void OnEvent(Event& e); // Will be run each time an event is triggered by the window.

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        
        void QuitApplication();
        
        Window& GetWindow() const { return *m_Window; }
        
    protected:
        virtual bool OnWindowResized(WindowResizeEvent& windowResizeEvent);
        virtual bool OnWindowClosed(WindowCloseEvent& windowCloseEvent);

    private:
        static Application* s_Instance;
        
        // std::string m_WindowName = "PurrKatEngine v" PKE_VERSION_STR;
        
        bool m_IsMinimized = false;
        bool m_IsRunning = true;
        
        Scope<Window> m_Window;
        LayerStack m_LayerStack;
        
        // Default Layer Stacks
        ImGuiLayer* m_ImGuiLayer;
        TimeManagerLayer* m_TimeManagerLayer;
    };

    // To be defined in a CLIENT.
    Application* CreateApplication();
}
