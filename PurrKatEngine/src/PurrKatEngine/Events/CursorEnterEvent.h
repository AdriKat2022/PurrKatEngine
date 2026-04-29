#pragma once

#include "pkepch.h"

#include "CursorEvent.h"
#include "Event.h"

namespace PurrKatEngine
{
    class CursorEnterEvent : public CursorEvent
    {
    public:
        CursorEnterEvent() { }

        EVENT_TO_STRING(CursorEnterEvent, "")
        
        EVENT_CLASS_TYPE(CursorEnter)
    };
}
