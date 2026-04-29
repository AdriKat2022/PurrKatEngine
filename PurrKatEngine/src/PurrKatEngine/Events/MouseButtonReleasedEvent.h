#pragma once

#include "pkepch.h"
#include "Event.h"
#include "MouseButtonEvent.h"

namespace PurrKatEngine
{
    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(int mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

        EVENT_TO_STRING(MouseButtonReleasedEvent, "ButtonCode={}", m_MouseButtonCode)
        
        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}
