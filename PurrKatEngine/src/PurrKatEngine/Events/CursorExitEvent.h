#pragma once

#include "pkepch.h"

#include "CursorEvent.h"
#include "Event.h"

namespace PurrKatEngine
{
    class CursorExitEvent : public CursorEvent
    {
    public:
        CursorExitEvent() { }

        EVENT_TO_STRING(CursorExitEvent, "")
        
        EVENT_CLASS_TYPE(CursorExit)
    };
}
