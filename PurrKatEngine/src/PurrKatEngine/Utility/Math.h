#pragma once

#include "glm/glm.hpp"

#define MATHF_CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define MATHF_CLAMP_01(x) ((x) < 0 ? 0 : ((x) > 1 ? 1 : (x)))

namespace PurrKatEngine
{
    class Math
    {
    public:
        static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
    };
}
