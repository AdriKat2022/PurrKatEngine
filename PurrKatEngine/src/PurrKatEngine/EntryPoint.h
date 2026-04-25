#pragma once

#ifdef PKE_PLATFORM_WINDOWS

extern PurrKatEngine::Application* PurrKatEngine::CreateApplication();

int main(int argc, char** argv)
{
    printf("Using PurrKatEngine <1.0>\n");
    auto app = PurrKatEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
