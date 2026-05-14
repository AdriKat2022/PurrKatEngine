#include "pkepch.h"
#include "StandardInputController.h"

#include "PurrKatEngine/Events/KeyPressedEvent.h"
#include "PurrKatEngine/Events/KeyReleasedEvent.h"
#include "PurrKatEngine/Inputs/Time.h"
#include "PurrKatEngine/Logs/InternalLog.h"

namespace PurrKatEngine
{
    void StandardInputController::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
        
        if (event.GetEventType() == EventType::KeyPressed)
        {
            KeyPressedEvent& keyPressedEvent = (KeyPressedEvent&)event;
            if (keyPressedEvent.IsRepeat()) return;
            if (keyPressedEvent.GetKeyCode() == KeyCode::W)
            {
                m_CurrentInput.y += 1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::S)
            {
                m_CurrentInput.y += -1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::A)
            {
                m_CurrentInput.x += -1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::D)
            {
                m_CurrentInput.x += 1;
            }
        }
        
        if (event.GetEventType() == EventType::KeyReleased)
        {
            KeyReleasedEvent& keyPressedEvent = (KeyReleasedEvent&)event;
            if (keyPressedEvent.GetKeyCode() == KeyCode::W)
            {
                m_CurrentInput.y -= 1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::S)
            {
                m_CurrentInput.y -= -1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::A)
            {
                m_CurrentInput.x -= -1;
            }
            if (keyPressedEvent.GetKeyCode() == KeyCode::D)
            {
                m_CurrentInput.x -= 1;
            }
        }
    }

    void StandardInputController::OnUpdate()
    {
        Layer::OnUpdate();
        
        m_OnInputCallback(m_CurrentInput.x * m_Speed, m_CurrentInput.y * m_Speed);
    }
}
