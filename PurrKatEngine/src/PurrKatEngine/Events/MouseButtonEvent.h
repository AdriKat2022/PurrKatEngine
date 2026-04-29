#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class MouseButtonEvent : public Event
    {
    public:
        explicit MouseButtonEvent(int mouseButtonCode) : m_MouseButtonCode(mouseButtonCode) {}

        int GetMouseButton() const { return m_MouseButtonCode; }
        
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryMouse)

    protected:
        int m_MouseButtonCode;
    };
}
