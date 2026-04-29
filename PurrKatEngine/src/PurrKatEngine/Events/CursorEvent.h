#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class CursorEvent : public Event
    {
    public:
        CursorEvent() { }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };
}
