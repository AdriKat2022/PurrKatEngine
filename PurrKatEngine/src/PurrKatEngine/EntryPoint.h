#pragma once

#include "Logs/InternalLog.h"

constexpr const char* VERSION = "1.0";

#ifdef PKE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    PurrKatEngine::InternalLog::Init();
    PKE_CORE_TRACE("Using PurrKatEngine version <{}>", VERSION);
    PKE_CORE_INFO("Initialized logging.");
    
    auto app = PurrKatEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
