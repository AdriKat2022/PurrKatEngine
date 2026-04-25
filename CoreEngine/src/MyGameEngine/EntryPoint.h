#pragma once

#ifdef MGE_PLATFORM_WINDOWS

extern MyGameEngine::Application* MyGameEngine::CreateApplication();

int main(int argc, char** argv)
{
    printf("Using MyGameEngine <1.0>\n");
    auto app = MyGameEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
