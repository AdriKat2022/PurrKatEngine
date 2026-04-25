#include <MyGameEngine.h>

class Sandbox : public MyGameEngine::Application
{
public:
    Sandbox() {}
    ~Sandbox() {}
};

MyGameEngine::Application* MyGameEngine::CreateApplication()
{
    return new Sandbox();
}