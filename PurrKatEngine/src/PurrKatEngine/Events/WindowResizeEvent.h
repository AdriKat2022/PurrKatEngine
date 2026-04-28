#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(const unsigned int width, const unsigned int height) : m_Width(width), m_Height(height) { }

        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

        EVENT_TO_STRING(WindowResizeEvent, "Width={} Height={}", m_Width, m_Height)

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
        
    private:
        unsigned int m_Width, m_Height;
    };
}
