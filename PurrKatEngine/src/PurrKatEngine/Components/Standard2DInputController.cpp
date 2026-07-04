#include "pkepch.h"
#include "Standard2DInputController.h"

#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Inputs/Time.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    void Standard2DInputController::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        
        if (event.GetEventType() == EventType::KeyPressed)
        {
            KeyPressedEvent& keyPressedEvent = (KeyPressedEvent&)event;
            if (keyPressedEvent.IsRepeat()) return;
            if (keyPressedEvent.GetKeyCode() == m_VerticalPositiveKey)
            {
                m_CurrentInput.y += 1;
            }
            if (keyPressedEvent.GetKeyCode() == m_VerticalNegativeKey)
            {
                m_CurrentInput.y += -1;
            }
            if (keyPressedEvent.GetKeyCode() == m_HorizontalNegativeKey)
            {
                m_CurrentInput.x += -1;
            }
            if (keyPressedEvent.GetKeyCode() == m_HorizontalPositiveKey)
            {
                m_CurrentInput.x += 1;
            }
        }
        
        if (event.GetEventType() == EventType::KeyReleased)
        {
            KeyReleasedEvent& keyPressedEvent = (KeyReleasedEvent&)event;
            if (keyPressedEvent.GetKeyCode() == m_VerticalPositiveKey)
            {
                m_CurrentInput.y -= 1;
            }
            if (keyPressedEvent.GetKeyCode() == m_VerticalNegativeKey)
            {
                m_CurrentInput.y -= -1;
            }
            if (keyPressedEvent.GetKeyCode() == m_HorizontalNegativeKey)
            {
                m_CurrentInput.x -= -1;
            }
            if (keyPressedEvent.GetKeyCode() == m_HorizontalPositiveKey)
            {
                m_CurrentInput.x -= 1;
            }
        }
    }

    void Standard2DInputController::OnUpdate()
    {
        Layer::OnUpdate();
        
        m_OnInputCallback.Invoke({m_CurrentInput.x * m_Speed, m_CurrentInput.y * m_Speed});
    }
}
