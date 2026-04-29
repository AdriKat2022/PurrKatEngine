#pragma once

#include "pkepch.h"

#include "CursorEvent.h"
#include "Event.h"

namespace PurrKatEngine
{
    class CursorPosEvent : public CursorEvent
    {
    public:
        CursorPosEvent(float mouseX, float mouseY): m_MouseX(mouseX), m_MouseY(mouseY) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }
        
        EVENT_TO_STRING(CursorPosEvent, "{}, {}", m_MouseX, m_MouseY)

        EVENT_CLASS_TYPE(MouseMoved)

    private:
        float m_MouseX, m_MouseY;
    };
}
