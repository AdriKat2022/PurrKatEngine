#include "pkepch.h"
#include "InternalLog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace PurrKatEngine
{
    std::shared_ptr<spdlog::logger> InternalLog::m_CoreLogger;
    std::shared_ptr<spdlog::logger> InternalLog::m_ClientLogger;
    
    void InternalLog::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        m_CoreLogger = spdlog::stdout_color_mt("PurrKatEngine");
        m_CoreLogger->set_level(spdlog::level::trace);
        
        m_ClientLogger = spdlog::stdout_color_mt("App");
        m_ClientLogger->set_level(spdlog::level::trace);
    }
    
}
