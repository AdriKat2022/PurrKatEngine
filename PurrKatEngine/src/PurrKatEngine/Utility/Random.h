#pragma once
#include <random>

#define PKE_RANDOM(min, max) ::PurrKatEngine::Random::Float(min, max);
#define PKE_RANDOM_INT(min, max) ::PurrKatEngine::Random::Int(min, max);

namespace PurrKatEngine
{
    class Random
    {
    public:
        static void Init() { s_RandomEngine.seed(std::random_device()()); }
        
        static float Float() { return (float)s_Distribution(s_RandomEngine)/(float)std::numeric_limits<uint32_t>::max(); }
        static float Float(float min, float max) { return std::lerp(min, max, Float()); }
        
        static int Int(int max) { return (int)std::floor(std::lerp(0, max, Float())); }
        static int Int(int min, int max) { return (int)std::floor(std::lerp(min, max, Float())); }
        
    private:
        static std::mt19937 s_RandomEngine;
        static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
    };
}
