#pragma once

#include "pkepch.h"

#include "CursorEvent.h"
#include "Event.h"

namespace PurrKatEngine
{
    class CursorPosEvent : public CursorEvent
    {
    public:
        CursorPosEvent(double mouseX, double mouseY): m_MouseX(mouseX), m_MouseY(mouseY) {}

        double GetX() const { return m_MouseX; }
        double GetY() const { return m_MouseY; }
        
        EVENT_TO_STRING(CursorPosEvent, "{}, {}", m_MouseX, m_MouseY)

        EVENT_CLASS_TYPE(MouseMoved)

    private:
        double m_MouseX, m_MouseY;
    };
}
