#pragma once

#include "PurrKatEngine/Events/Event.h"

namespace PurrKatEngine
{
    class PKE_API Layer
    {
    public:
        explicit Layer(const std::string& name = "Layer") : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}
        
    protected:
        std::string m_DebugName;
    };
    
}
