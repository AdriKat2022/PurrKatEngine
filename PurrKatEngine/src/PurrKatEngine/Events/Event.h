#pragma once

#include "pkepch.h"
#include "PurrKatEngine/Core.h"

namespace PurrKatEngine
{
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        CursorEnter, CursorExit
    };

    // Bit field for event categories allowing for multiple categories to be applied to a single event
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    // Both of the following macros help reduce repeated written code and should be used in every EventClass.
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
                                virtual EventType GetEventType() const override { return GetStaticType(); } \
                                virtual const char* GetName() const override { return #type; }
    
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

#define EVENT_TO_STRING(classname, fmt_str, ...) std::string ToString() const override { return std::format("{} " fmt_str, #classname, __VA_ARGS__); }

#define DECLARE_EVENT_TYPE(eventType) bool On##eventType(eventType& e);
    
    // Base class for all events in the engine
    class PKE_API Event
    {
        friend class EventDispatcher;

    protected:
        Event() = default;
        
    public:
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0; // Purely for debugging purposes, overhead is minimal so kept for now.
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(const EventCategory category) const { return GetCategoryFlags() & category; }
        bool IsHandled() const { return m_Handled; }
        
    protected:
        bool m_Handled = false;
    };

    class EventDispatcher
    {
#define DISPATCH(eventName, functionName) Dispatch<eventName>(PKE_BIND_FUNCTION(functionName))
#define DISPATCH_ON(eventName) Dispatch<eventName>(PKE_BIND_FUNCTION(On##eventName))
        
        template<typename T>
        using EventFunction = std::function<bool(T&)>;

    public:
        explicit EventDispatcher(Event& event) : m_Event(event) {}

        // Returns true if the event was successfully dispatched with the event function. False otherwise.
        template<typename T>
        bool Dispatch(EventFunction<T> eventFunction)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled = eventFunction(*(T*)&m_Event);
                return true;
            }
            return false;
        }
        
    private:
        Event& m_Event;
    };

    // For the logging library to function correctly.
    inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }
    inline std::string format_as(const Event& e) { return e.ToString(); }
}