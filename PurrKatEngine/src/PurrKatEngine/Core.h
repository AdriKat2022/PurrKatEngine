#pragma once

#ifdef PKE_PLATFORM_WINDOWS
    #ifdef PKE_BUILD_DLL
        #define PKE_API __declspec(dllexport)
    #else
        #define PKE_API __declspec(dllimport)
    #endif
#else
    #error MyGameEngine only supports windows for now!
#endif

#ifdef PKE_ENABLE_ASSERTS
    #define PKE_CORE_ASSERT(condition, ...) { if (!(condition)) { PKE_CORE_ERROR("Assertion error: {0}", __VA_ARGS__); __debugbreak(); } }
    #define PKE_ASSERT(condition, ...) { if (!(condition)) { PKE_ERROR("Assertion error: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define PKE_CORE_ASSERT(condition, ...)
    #define PKE_ASSERT(condition, ...)
#endif

#define BIT(x) (1 << (x))

#define PKE_BIND_FUNCTION(memberFunction) [this](auto&&... args) -> decltype(auto) \
    { return this->memberFunction(std::forward<decltype(args)>(args)...); }