#pragma once

#ifdef PKE_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    PurrKatEngine::Log::Init();
    PurrKatEngine::Log::GetCoreLogger()->info("Using PurrKatEngine <2.0>");
    PurrKatEngine::Log::GetCoreLogger()->warn("Initialized logging.");
    PurrKatEngine::Log::GetClientLogger()->info("Done.");
    
    auto app = PurrKatEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
