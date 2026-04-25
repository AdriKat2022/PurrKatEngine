#pragma once

constexpr const char* VERSION = "1.0";

#ifdef PKE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    PurrKatEngine::Log::Init();
    PKE_CORE_INFO("Using PurrKatEngine version <{0}>", VERSION);
    PKE_CORE_WARN("Initialized logging.");
    
    auto app = PurrKatEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
