#pragma once
#include "PurrKatEngine/Inputs/Keys.h"
#include "PurrKatEngine/Layers/Layer.h"
#include "PurrKatEngine/Utility/EventAction.h"

namespace PurrKatEngine
{
    /// A layer that listens for WASD input and calls a callback with the current input, multiplied by a speed.
    class Standard2DInputController : public Layer
    {
    public:
        using InputCallbackType = std::function<void(glm::vec2)>;
        
        explicit Standard2DInputController() = default;
        
        explicit Standard2DInputController(KeyCode xNegativeKey, KeyCode xPositiveKey, KeyCode yNegativeKey, KeyCode yPositiveKey)
            : m_HorizontalNegativeKey(xNegativeKey), m_HorizontalPositiveKey(xPositiveKey), m_VerticalNegativeKey(yNegativeKey), m_VerticalPositiveKey(yPositiveKey) {}
        
        explicit Standard2DInputController(const InputCallbackType& onInputCallback)
            : m_OnInputCallback(onInputCallback) {}

        explicit Standard2DInputController(const InputCallbackType& onInputCallback, KeyCode xNegativeKey, KeyCode xPositiveKey, KeyCode yNegativeKey, KeyCode yPositiveKey)
            : m_OnInputCallback(onInputCallback), m_HorizontalNegativeKey(xNegativeKey), m_HorizontalPositiveKey(xPositiveKey), m_VerticalNegativeKey(yNegativeKey), m_VerticalPositiveKey(yPositiveKey) {}

        void ClearCallbacks() { m_OnInputCallback.ClearListeners(); }
        void AddCallback(const InputCallbackType& callback) { m_OnInputCallback.AddListener(callback); }
        
        void SetSpeed(float speed) { m_InputAmplitude = speed; }

        void OnEvent(Event& event) override;
        void OnUpdate() override;
        
        glm::vec2 GetInput() const { return m_CurrentInput; }

    private:
        float m_InputAmplitude = 1.0f;
        glm::vec2 m_CurrentInput = {0, 0};
        
        EventAction<glm::vec2> m_OnInputCallback;
        
        KeyCode m_HorizontalNegativeKey = KeyCode::A;
        KeyCode m_HorizontalPositiveKey = KeyCode::D;
        KeyCode m_VerticalNegativeKey = KeyCode::S;
        KeyCode m_VerticalPositiveKey = KeyCode::W;
    };
}
