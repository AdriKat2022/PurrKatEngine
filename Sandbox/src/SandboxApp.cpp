#include <PurrKatEngine.h>
#include <PurrKatEngine/EntryPoint.h>

#include "Sandbox2DLightTestScene.h"
#include "Sandbox2DMapTiling.h"

class SandboxApp : public PKE::Application
{
public:
    SandboxApp()
    {
        PKE_LOG_TRACE("Sandbox application start. Hey, that's me! A log from the client!");
        PushLayer(new Sandbox2DLightTestScene());
        
        // auto texture = PurrKatEngine::Texture2D::CreateRef("assets/textures/TileSets/Tilled_Dirt_Wide.png", { .Filter = PurrKatEngine::Texture2D::FilterType::Nearest });
        // PushLayer(new PurrKatEngine::SpriteSheetLayerDebugging(texture));
    }
};

PKE_RUN_CLASS(SandboxApp);
