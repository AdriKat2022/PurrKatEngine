#include "pkepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "PurrKatEngine/Application.h"

namespace PurrKatEngine
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetProjection(float x, float x1, float x2, float x3) {
        m_ProjectionMatrix = glm::ortho(x, x1, x2, x3);
        RecalculateViewMatrix();
    }

    glm::vec3 OrthographicCamera::ScreenToWorldPosition(const glm::vec2& screenPosition) const
    {
        const Window& window = Application::Get().GetWindow();
        const float width = static_cast<float>(window.GetWidth());
        const float height = static_cast<float>(window.GetHeight());

        if (width <= 0.0f || height <= 0.0f)
            return m_Position;

        const glm::vec2 normalizedDevicePosition = {
            2.0f * screenPosition.x / width - 1.0f,
            1.0f - 2.0f * screenPosition.y / height
        };

        glm::vec4 worldPosition = glm::inverse(m_ViewProjectionMatrix)
            * glm::vec4(normalizedDevicePosition, 0.0f, 1.0f);
        worldPosition /= worldPosition.w;

        return worldPosition;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        glm::mat4 identityMatrix(1.0f);
        
        glm::mat4 transform =
            glm::translate(identityMatrix, m_Position)
          * glm::rotate(identityMatrix, m_Rotation.x, glm::vec3(1, 0, 0))
          * glm::rotate(identityMatrix, m_Rotation.y, glm::vec3(0, 1, 0))
          * glm::rotate(identityMatrix, m_Rotation.z, glm::vec3(0, 0, 1));
        
        m_ViewMatrix = glm::inverse(transform);
        
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
