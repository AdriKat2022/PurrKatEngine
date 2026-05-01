#pragma once

#include "pkepch.h"
#include "Event.h"
#include "PurrKatEngine/Inputs/Keys.h"

namespace PurrKatEngine
{
    class KeyEvent : public Event
    {
    public:
        explicit KeyEvent(unsigned int keyCode) : m_KeyCode(keyCode) {}

        unsigned int GetCharCode() const { return m_KeyCode; }
        KeyCode GetKeyCode() const { return GlfwCharCodeToKeyCode(m_KeyCode); }
        ImGuiKey GetImGuiKey() const { return GlfwCharCodeToImGuiKey(m_KeyCode); }
        
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        unsigned int m_KeyCode;
    };
}
