#include <PurrKatEngine.h>
#include <PurrKatEngine/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2DLightTestScene.h"

class SandboxApp : public PKE::Application
{
public:
    SandboxApp()
    {
        PKE_LOG_TRACE("Sandbox application start. Hey, that's me! A log from the client!");
        // PushLayer(new ExampleSandboxLayer());
        // PushLayer(new Sandbox2DLightTestScene());
        
        auto texture = PurrKatEngine::Texture2D::CreateRef("assets/textures/spriteSheet.png", { .Filter = PurrKatEngine::Texture2D::FilterType::Nearest });
        PushLayer(new PurrKatEngine::SpriteSheetLayerDebugging(texture));
    }
};

RUN_CLASS(SandboxApp);
