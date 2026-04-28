#pragma once

#include "pkepch.h"
#include "Event.h"
#include "KeyEvent.h"

namespace PurrKatEngine
{
    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(int keyCode, bool isRepeat = false) : KeyEvent(keyCode), m_IsRepeat(isRepeat) {}

        EVENT_TO_STRING(KeyPressedEvent, "KeyCode={} IsRepeat={}", m_KeyCode, m_IsRepeat)
        EVENT_CLASS_TYPE(KeyPressed)

    protected:
        bool m_IsRepeat;
    };
}
