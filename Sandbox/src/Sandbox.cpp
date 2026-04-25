#include <PurrKatEngine.h>

class Sandbox : public PurrKatEngine::Application
{
public:
    Sandbox() { printf("Hey that's me! A log from the client!"); }
    ~Sandbox() {}
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
