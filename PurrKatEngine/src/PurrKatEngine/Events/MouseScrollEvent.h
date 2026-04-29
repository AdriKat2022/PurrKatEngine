#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) { }

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        EVENT_TO_STRING(MouseScrollEvent, "XOffset={} YOffset={}", m_XOffset, m_YOffset)

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    private:
        float m_XOffset, m_YOffset;
    };
}
