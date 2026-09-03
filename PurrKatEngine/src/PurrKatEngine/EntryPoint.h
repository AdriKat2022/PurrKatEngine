#pragma once

#include "PurrKatEngine/Application.h"
#include "Logs/InternalLog.h"
#include "Profiling/Profiler.h"

#ifdef PKE_PLATFORM_WINDOWS

inline int main(int argc, char** argv)
{
    PROFILE_SESSION_BEGIN("Startup", "Profiling-Startup.json");
    
    PurrKatEngine::InternalLog::Init();
    PKE_CORE_TRACE("Using PurrKatEngine version <{}>", PKE_VERSION_STR);
    auto app = PurrKatEngine::CreateApplication({.Count = argc, .Args = argv});
    
    PROFILE_SESSION_END();
    PROFILE_SESSION_BEGIN("Runtime", "Profiling-Runtime.json");
    
    app->Run();
    
    PROFILE_SESSION_END();
    
    return 0;
}

#endif
