namespace MyGameEngine
{
    __declspec(dllimport) void Print();
}


int main(int argc, char* argv[])
{
    MyGameEngine::Print();
    
    return 0;
}
