#pragma once

#include "pkepch.h"
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include <spdlog/common.h>

namespace PurrKatEngine
{
    class PKE_API Log
    {
    public:
        template <typename... Args>
        static void LogCritical(spdlog::format_string_t<Args...> fmt, Args &&...args);
        template <typename... Args>
        static void LogError(spdlog::format_string_t<Args...> fmt, Args &&...args);
        template <typename... Args>
        static void LogWarn(spdlog::format_string_t<Args...> fmt, Args &&...args);
        template <typename... Args>
        static void LogInfo(spdlog::format_string_t<Args...> fmt, Args &&...args);
        template <typename... Args>
        static void LogDebug(spdlog::format_string_t<Args...> fmt, Args &&...args);
        template <typename... Args>
        static void LogTrace(spdlog::format_string_t<Args...> fmt, Args &&...args);
    };
}

// Template implementations must be in header for client instantiation
namespace PurrKatEngine
{
    template <typename ... Args>
    void Log::LogCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetCoreLogger()->critical(fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void Log::LogError(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetCoreLogger()->error(fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void Log::LogWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetCoreLogger()->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void Log::LogInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetCoreLogger()->info(fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void Log::LogDebug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetClientLogger()->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename ... Args>
    void Log::LogTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        InternalLog::GetClientLogger()->trace(fmt, std::forward<Args>(args)...);
    }
}

// CLIENT Logs Macros
#define PKE_LOG_CRITICAL(...)  PurrKatEngine::Log::LogCritical(__VA_ARGS__)
#define PKE_LOG_ERROR(...)     PurrKatEngine::Log::LogError(__VA_ARGS__)
#define PKE_LOG_WARN(...)      PurrKatEngine::Log::LogWarn(__VA_ARGS__)
#define PKE_LOG_INFO(...)      PurrKatEngine::Log::LogInfo(__VA_ARGS__)
#define PKE_LOG_DEBUG(...)     PurrKatEngine::Log::LogDebug(__VA_ARGS__)
#define PKE_LOG_TRACE(...)     PurrKatEngine::Log::LogTrace(__VA_ARGS__)