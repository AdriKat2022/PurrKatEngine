#include <MyGameEngine.h>

class Sandbox : public MyGameEngine::Application
{
    public:
    Sandbox()
    {
        
    }
    
    ~Sandbox() { }
};

int main()
{
    Sandbox sandbox;
    sandbox.Run();
}
