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
        void OnImGuiRender() override;

        void Begin();
        void End();
        
    private:
        float m_Time = 0.0f;
    };
}
