#pragma once

#include "glm/glm.hpp"

#define MATHF_MAX(a, b) ((a) > (b) ? (a) : (b))
#define MATHF_MIN(a, b) ((a) < (b) ? (a) : (b))
#define MATHF_CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define MATHF_CLAMP_01(x) ((x) < 0 ? 0 : ((x) > 1 ? 1 : (x)))

namespace PurrKatEngine
{
    class Math
    {
    public:
        static int Max(int a, int b);
        static int Min(int a, int b);
        
        static float Max(float a, float b);
        static float Min(float a, float b);
        
        static double Max(double a, double b);
        static double Min(double a, double b);
        
        static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
    };
}
