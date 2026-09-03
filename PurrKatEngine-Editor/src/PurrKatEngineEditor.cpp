#include <PurrKatEngine.h>
#include <PurrKatEngine/EntryPoint.h>
#include "EditorViewLayer.h"

namespace PurrKatEngine
{
    class PurrKatEngineEditor : public Application
    {
    public:
        PurrKatEngineEditor(ApplicationCommandLineArgs args) : Application("PurrKatEngine Editor v" PKE_VERSION_STR, args)
        {
            PushLayer(new EditorViewLayer());
        }
    };
    
    PKE_RUN_CLASS(PurrKatEngineEditor)
}
