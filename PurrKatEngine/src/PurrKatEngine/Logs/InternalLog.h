#pragma once

#include "PurrKatEngine/Core.h"
#include "spdlog/spdlog.h"

namespace PurrKatEngine
{
    class PKE_API InternalLog
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

// CORE Logs Macros
#define PKE_CORE_CRITICAL(...)  PurrKatEngine::InternalLog::GetCoreLogger()->critical(__VA_ARGS__)
#define PKE_CORE_ERROR(...)     PurrKatEngine::InternalLog::GetCoreLogger()->error(__VA_ARGS__)
#define PKE_CORE_WARN(...)      PurrKatEngine::InternalLog::GetCoreLogger()->warn(__VA_ARGS__)
#define PKE_CORE_INFO(...)      PurrKatEngine::InternalLog::GetCoreLogger()->info(__VA_ARGS__)
#define PKE_CORE_DEBUG(...)     PurrKatEngine::InternalLog::GetCoreLogger()->debug(__VA_ARGS__)
#define PKE_CORE_TRACE(...)     PurrKatEngine::InternalLog::GetCoreLogger()->trace(__VA_ARGS__)


