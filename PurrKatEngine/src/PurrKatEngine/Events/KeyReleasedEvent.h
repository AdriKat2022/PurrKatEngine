#pragma once

#include "pkepch.h"
#include "Event.h"
#include "KeyEvent.h"

namespace PurrKatEngine
{
    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(unsigned int keyCode) : KeyEvent(keyCode) {}

        EVENT_TO_STRING(KeyReleasedEvent, "KeyCode={}", m_KeyCode)
        EVENT_CLASS_TYPE(KeyReleased)
    };
}
