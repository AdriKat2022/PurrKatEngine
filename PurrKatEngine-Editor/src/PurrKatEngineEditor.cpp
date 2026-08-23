#include <PurrKatEngine.h>
#include <PurrKatEngine/EntryPoint.h>
#include "EditorViewLayer.h"

namespace PurrKatEngine
{
    class PurrKatEngineEditor : public Application
    {
    public:
        PurrKatEngineEditor() : Application("PurrKatEngineEditor v" PKE_VERSION_STR)
        {
            PushLayer(new EditorViewLayer());
        }
    };
    
    PKE_RUN_CLASS(PurrKatEngineEditor)
}
