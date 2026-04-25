#include <MyGameEngine.h>

class Sandbox : public MyGameEngine::Application
{
public:
    Sandbox() { printf("Hey that's me!"); }
    ~Sandbox() {}
};

CREATE_APPLICATION_FROM_CLASS(Sandbox)
