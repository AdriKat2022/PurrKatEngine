#include <PurrKatEngine.h>

class Sandbox : public PurrKatEngine::Application
{
public:
    Sandbox() { PKE_LOG_TRACE("Sandbox application start. Hey, that's me! A log from the client!"); }
    ~Sandbox() {}
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
