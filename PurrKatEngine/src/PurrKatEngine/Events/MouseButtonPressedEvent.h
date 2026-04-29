#pragma once

#include "pkepch.h"
#include "Event.h"
#include "MouseButtonEvent.h"

namespace PurrKatEngine
{
    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(int mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

        EVENT_TO_STRING(MouseButtonPressedEvent, "ButtonCode={}", m_MouseButtonCode)
        
        EVENT_CLASS_TYPE(MouseButtonPressed)
    };
}
