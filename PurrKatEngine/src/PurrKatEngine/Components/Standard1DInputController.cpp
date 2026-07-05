#include "pkepch.h"
#include "Standard1DInputController.h"

#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Inputs/Time.h"

namespace PurrKatEngine
{
    void Standard1DInputController::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        
        if (event.GetEventType() == EventType::KeyPressed)
        {
            KeyPressedEvent& keyPressedEvent = (KeyPressedEvent&)event;
            if (keyPressedEvent.IsRepeat()) return;
            if (keyPressedEvent.GetKeyCode() == m_PositiveKey)
            {
                m_CurrentInput += 1;
            }
            if (keyPressedEvent.GetKeyCode() == m_NegativeKey)
            {
                m_CurrentInput -= 1;
            }
        }
        
        if (event.GetEventType() == EventType::KeyReleased)
        {
            KeyReleasedEvent& keyPressedEvent = (KeyReleasedEvent&)event;
            if (keyPressedEvent.GetKeyCode() == m_PositiveKey)
            {
                m_CurrentInput += -1;
            }
            if (keyPressedEvent.GetKeyCode() == m_NegativeKey)
            {
                m_CurrentInput -= -1;
            }
        }
    }

    void Standard1DInputController::OnUpdate()
    {
        Layer::OnUpdate();
        
        m_OnInputCallback.Invoke(m_CurrentInput * m_InputAmplitude);
    }
}
