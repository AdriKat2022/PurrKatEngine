#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class KeyEvent : public Event
    {
    public:
        explicit KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
        
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        int m_KeyCode;
    };
}
