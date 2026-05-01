#pragma once
#include "KeyEvent.h"

namespace PurrKatEngine
{
    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(unsigned int keyCode) : KeyEvent(keyCode) {}

        EVENT_TO_STRING(KeyTypedEvent, "KeyCode={}", m_KeyCode)
        EVENT_CLASS_TYPE(KeyTyped)
    };
}
