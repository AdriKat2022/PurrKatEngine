#pragma once
#include "PurrKatEngine/Inputs/Keys.h"
#include "PurrKatEngine/Layers/Layer.h"
#include "PurrKatEngine/Utility/EventAction.h"

namespace PurrKatEngine
{
    /// A layer that listens for WASD input and calls a callback with the current input, multiplied by a speed.
    class Standard1DInputController : public Layer
    {
    public:
        using InputCallbackType = std::function<void(float)>;
        
        explicit Standard1DInputController() = default;
        
        explicit Standard1DInputController(KeyCode negativeKey, KeyCode positiveKey)
            : m_NegativeKey(negativeKey), m_PositiveKey(positiveKey) {}
        
        explicit Standard1DInputController(const InputCallbackType& onInputCallback)
            : m_OnInputCallback(onInputCallback) {}

        explicit Standard1DInputController(const InputCallbackType& onInputCallback, KeyCode negativeKey, KeyCode positiveKey)
            : m_OnInputCallback(onInputCallback), m_NegativeKey(negativeKey), m_PositiveKey(positiveKey) {}

        void ClearCallbacks() { m_OnInputCallback.ClearListeners(); }
        void AddCallback(const InputCallbackType& callback) { m_OnInputCallback.AddListener(callback); }
        
        void SetSpeed(float speed) { m_InputAmplitude = speed; }

        void OnEvent(Event& event) override;
        void OnUpdate() override;
        
        float GetInput() const { return m_CurrentInput; }

    private:
        float m_InputAmplitude = 1.0f;
        float m_CurrentInput = 0.0f;
        
        EventAction<float> m_OnInputCallback;
        
        KeyCode m_NegativeKey = KeyCode::A;
        KeyCode m_PositiveKey = KeyCode::D;
    };
}
