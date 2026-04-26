#pragma once

#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Events/Event.h"

#define CREATE_WINDOW_SETUP(windowType) Window* Window::Create(const WindowProps& props) { return new windowType(props); }

namespace PurrKatEngine
{
    struct WindowProps
    {
        std::string Title = "PurrKatEngine";
        unsigned int Width = 1280;
        unsigned int Height = 720;

        WindowProps() = default;
        explicit WindowProps(
            const std::string& title,
            const unsigned int width = 1280,
            const unsigned int height = 720)
            : Title(title), Width(width), Height(height) {}
    };
    
    class PKE_API Window
    {
    public:
        using EventCallbackFunction = std::function<void(Event&)>;

        virtual ~Window() {}
        
        virtual void OnUpdate() = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}
