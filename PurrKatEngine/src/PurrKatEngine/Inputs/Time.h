#pragma once
#include "PurrKatEngine/Layers/Layer.h"

namespace PurrKatEngine
{
    class Time
    {
    public:
        static double time; // Time since application startup.
        // static double deltaTime;
    };
    
    class TimeManagerLayer : public Layer
    {
    public:
        ~TimeManagerLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;
    };
}
