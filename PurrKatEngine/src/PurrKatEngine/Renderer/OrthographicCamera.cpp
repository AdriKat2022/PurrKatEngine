#include "pkepch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace PurrKatEngine
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        RecalculateViewMatrix();
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
