#pragma once

#include "Logs/InternalLog.h"
#include "Profiling/Profiler.h"

constexpr const char* VERSION = "1.0";

#ifdef PKE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    PROFILE_SESSION_BEGIN("Startup", "Profiling-Startup.json");
    
    PurrKatEngine::InternalLog::Init();
    PKE_CORE_TRACE("Using PurrKatEngine version <{}>", VERSION);
    auto app = PurrKatEngine::CreateApplication();
    
    PROFILE_SESSION_END();
    PROFILE_SESSION_BEGIN("Runtime", "Profiling-Runtime.json");
    
    app->Run();
    
    PROFILE_SESSION_END();
    
    return 0;
}

#endif
