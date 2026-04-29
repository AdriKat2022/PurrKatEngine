#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(const double xOffset, const double yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) { }

        double GetXOffset() const { return m_XOffset; }
        double GetYOffset() const { return m_YOffset; }

        EVENT_TO_STRING(MouseScrollEvent, "XOffset={} YOffset={}", m_XOffset, m_YOffset)

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    private:
        double m_XOffset, m_YOffset;
    };
}
