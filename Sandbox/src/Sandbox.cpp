#include <PurrKatEngine.h>

class ExampleSandboxLayer : public PurrKatEngine::Layer
{
public:
    ExampleSandboxLayer() : Layer("ExampleSandboxLayer") {}

    void OnUpdate() override
    {
        PurrKatEngine::Log::LogDebug("ExampleSandboxLayer::OnUpdate");
    }

    void OnEvent(PurrKatEngine::Event& e) override
    {
        PKE_LOG_DEBUG("ExampleSandboxLayer::OnEvent {}", e.ToString());
    }
};

class Sandbox : public PurrKatEngine::Application
{
public:
    Sandbox()
    {
        PurrKatEngine::Log::LogTrace("Sandbox application start. Hey, that's me! A log from the client!");
        PushLayer(new ExampleSandboxLayer());
        PushOverlay(new PurrKatEngine::ImGuiLayer());
    }
};

CREATE_APPLICATION_FROM_CLASS(Sandbox);
