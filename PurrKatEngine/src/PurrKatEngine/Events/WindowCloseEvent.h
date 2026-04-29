#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_TO_STRING(WindowCloseEvent)

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}
