#pragma once
#include "PurrKatEngine/Events/ApplicationEvents.h"
#include "PurrKatEngine/Layers/Layer.h"

namespace PurrKatEngine
{
    class PKE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void Begin();
        void End();
        
        void SetBlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }
        
    private:
        bool m_BlockEvents = false;
        float m_Time = 0.0f;
    };
}
