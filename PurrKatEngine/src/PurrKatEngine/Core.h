#pragma once

#include <memory>

#ifdef PKE_PLATFORM_WINDOWS
    #ifdef PKE_DYNAMIC_LIBRARY
        #ifdef PKE_BUILD_DLL
            #define PKE_API __declspec(dllexport)
        #else
            #define PKE_API __declspec(dllimport)
        #endif
    #else
        #define PKE_API
    #endif
#else
    #error PurrKatEngine only supports windows for now!
#endif

#ifdef PKE_ENABLE_ASSERTS
    #define PKE_CORE_ASSERT(condition, ...) { if (!(condition)) { PKE_CORE_ERROR("Assertion error: {}", fmt::format(__VA_ARGS__)); __debugbreak(); } }
    #define PKE_ASSERT(condition, ...) { if (!(condition)) { PKE_ERROR("Assertion error: {}", fmt::format(__VA_ARGS__)); __debugbreak(); } }
#else
    #define PKE_CORE_ASSERT(condition, ...)
    #define PKE_ASSERT(condition, ...)
#endif

#define BIT(x) (1 << (x))

#define PKE_BIND_FUNCTION(memberFunction) [this](auto&&... args) -> decltype(auto) \
    { return this->memberFunction(std::forward<decltype(args)>(args)...); }

namespace PurrKatEngine
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    Ref<T> MakeRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    template<typename T, typename... Args>
    Ref<T> ToRef(T* obj) { return Ref<T>(obj); }
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T, typename... Args>
    Scope<T> ToScope(T* obj) { return Scope<T>(obj); }
}
