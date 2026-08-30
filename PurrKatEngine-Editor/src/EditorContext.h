#pragma once
#include <string>
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Scene/Scene.h"

namespace PurrKatEngine
{
    class EditorContext
    {
    public:
        EditorContext();
        EditorContext(const std::string& openScene);
        
        Ref<Scene> GetScene() const { return m_ActiveScene; }
        const std::string& GetActiveSceneFilePath() const { return m_ActiveSceneFilePath; }
        std::string GetActiveSceneName() const;

        void NewScene();
        bool OpenScene(const std::string& sceneToOpen = {});
        bool SaveScene();
        bool SaveSceneAs(const std::string& newSceneName = {});

    private:
        Ref<Scene> m_ActiveScene = nullptr;
        std::string m_ActiveSceneFilePath = {};
    };
}
