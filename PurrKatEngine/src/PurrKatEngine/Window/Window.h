#pragma once

#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Events/Event.h"

#define PKE_CREATE_WINDOW_SETUP(windowType) Window* Window::Create(const WindowProps& props) { return new windowType(props); }

namespace PurrKatEngine
{
    struct WindowProps
    {
        std::string Title = "PurrKatEngine Window";
        uint32_t Width = 1280;
        uint32_t Height = 720;
    };

    // Acts as an interface for a window (has to be implemented per platform)
    class PKE_API Window
    {
    public:
        using EventCallbackFunction = std::function<void(Event&)>;

        virtual ~Window() {}
        
        virtual void OnUpdate() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;
        
        static Window* Create(const WindowProps& props = {});
    };
}
