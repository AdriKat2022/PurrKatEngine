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

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowsResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
        
    private:
        unsigned int m_Width, m_Height;
    };
}
