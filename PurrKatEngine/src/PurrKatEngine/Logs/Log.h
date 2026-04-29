#pragma once

#include "PurrKatEngine/Core.h"
#include "spdlog/spdlog.h"

namespace PurrKatEngine
{
    class PKE_API Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_CoreLogger;
        static std::shared_ptr<spdlog::logger> m_ClientLogger;
    };
}

// #define FORMAT_VARS(...) std::format(Make_Fmt(#__VA_ARGS__), __VA_ARGS__)
// #define FORMAT_VARS_STRING(...) Make_Fmt(#__VA_ARGS__), __VA_ARGS__

// CORE Logs Macros
#define PKE_CORE_CRITICAL(...)  PurrKatEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define PKE_CORE_ERROR(...)     PurrKatEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PKE_CORE_WARN(...)      PurrKatEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PKE_CORE_INFO(...)      PurrKatEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PKE_CORE_DEBUG(...)     PurrKatEngine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define PKE_CORE_TRACE(...)     PurrKatEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

// CLIENT Logs Macros
#define PKE_LOG_CRITICAL(...)  PurrKatEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define PKE_LOG_ERROR(...)     PurrKatEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define PKE_LOG_WARN(...)      PurrKatEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PKE_LOG_INFO(...)      PurrKatEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define PKE_LOG_DEBUG(...)     PurrKatEngine::Log::GetClientLogger()->debug(__VA_ARGS__)
#define PKE_LOG_TRACE(...)     PurrKatEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
