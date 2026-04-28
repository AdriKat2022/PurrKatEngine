#pragma once

#include "Window.h"
#include "GLFW/glfw3.h"

namespace PurrKatEngine
{
    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void OnUpdate() override;
        
        unsigned int GetWidth() const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        std::string ToString() const { return (std::stringstream()<<m_Data.Title<<" Window : "<<GetWidth()<<"x"<<GetHeight()).str(); }
        bool IsVSync() const override { return m_Data.VSync; }
        
        void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(const bool enabled) override;
        
    private:
        GLFWwindow* m_Window;

        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
        
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFunction EventCallback;
        };

        WindowData m_Data;
    };
}
