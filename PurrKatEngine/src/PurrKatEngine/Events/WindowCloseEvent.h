#pragma once

#include "pkepch.h"
#include "Event.h"

namespace PurrKatEngine
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowsCloseEvent";
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}
