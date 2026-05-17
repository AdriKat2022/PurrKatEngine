#pragma once
#include "PurrKatEngine/Layers/Layer.h"

namespace PurrKatEngine
{
    /// A layer that listens for WASD input and calls a callback with the current input, multiplied by a speed.
    class StandardInputController : public Layer
    {
    public:
        explicit StandardInputController(const std::function<void(float,float)>& onInputCallback) : m_OnInputCallback(onInputCallback) {}
        
        void SetSpeed(float speed) { m_Speed = speed; }

        void OnEvent(Event& event) override;
        void OnUpdate() override;

    private:
        float m_Speed = 1.0f;
        glm::vec2 m_CurrentInput = {0, 0};
        std::function<void(float,float)> m_OnInputCallback;
    };
}
