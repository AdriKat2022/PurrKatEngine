#include "pkepch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PurrKatEngine
{
    int Math::Max(int a, int b) { return (a > b) ? a : b; }
    int Math::Min(int a, int b) { return (a < b) ? a : b; }
    
    float Math::Max(float a, float b) { return (a > b) ? a : b; }
    float Math::Min(float a, float b) { return (a < b) ? a : b; }

    double Math::Max(double a, double b) { return (a > b) ? a : b; }
    double Math::Min(double a, double b) { return (a < b) ? a : b; }

    bool Math::DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        glm::mat4 localMatrix(transform);
        
        // Normalize the matrix.
        if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>()))
            return false;
        
        // First, isolate perspective. This is the messiest.
        if (
            glm::epsilonNotEqual(localMatrix[0][3], 0.0f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[1][3], 0.0f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[2][3], 0.0f, glm::epsilon<float>()))
        {
            // Clear the perspective partition
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
            localMatrix[3][3] = 1.0f;
        }
        
        // Next take care of translation (easy).
        translation = glm::vec3(localMatrix[3]);
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);
        
        glm::vec3 row[3], pdum3;
        
        // Now get scale and shear.
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                row[i][j] = localMatrix[i][j];
        
        // Compute X scale factor and normalize first row.
        scale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], 1.0f);
        scale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], 1.0f);
        scale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], 1.0f);
        
        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
#if false
        pdum3 = glm::cross(row[1], row[2]);
        if (glm::dot(row[0], pdum3) < 0)
        {
            for (int i = 0; i < 3; i++)
            {
                scale[i] *= -1.0f;
                row[i] *= -1.0f;
            }
        }
#endif
        
        rotation.y = asin(-row[0][2]);
        if (cos(rotation.y) != 0)
        {
            rotation.x = atan2(row[1][2], row[2][2]);
            rotation.z = atan2(row[0][1], row[0][0]);
        }
        else
        {
            rotation.x = atan2(-row[2][0], row[1][1]);
            rotation.z = 0;
        }
        
        return true;
    }
}
