#pragma once

#ifdef MGE_PLATFORM_WINDOWS

extern MyGameEngine::Application* MyGameEngine::CreateApplication();

int main(int argc, char** argv)
{
    auto app = MyGameEngine::CreateApplication();
    app->Run();
    return 0;
}

#endif
