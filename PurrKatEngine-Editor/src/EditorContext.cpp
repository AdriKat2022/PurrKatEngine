#include "EditorContext.h"

#include "PurrKatEngine/Constants.h"
#include "PurrKatEngine/PlatformUtils.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Serialization/SceneSerializer.h"

namespace PurrKatEngine
{
    EditorContext::EditorContext()
    {
        m_ActiveScene = MakeRef<Scene>();
    }

    EditorContext::EditorContext(const std::string& openScene)
    {
        m_ActiveScene = MakeRef<Scene>();
        OpenScene(openScene);
    }

    std::string EditorContext::GetActiveSceneName() const
    {
        if (m_ActiveSceneFilePath.empty())
            return Constants::DefaultSceneName;
        
        size_t lastSlashPos = m_ActiveSceneFilePath.find_last_of("/\\");
        size_t lastDotPos = m_ActiveSceneFilePath.find_last_of('.');
        
        if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos)
            return Constants::DefaultSceneName;
        
        static std::string sceneName;
        sceneName = m_ActiveSceneFilePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
        return sceneName;
    }

    void EditorContext::NewScene()
    {
        m_ActiveSceneFilePath.clear();
        m_ActiveScene->EmptyScene();
    }

    bool EditorContext::OpenScene(const std::string& sceneToOpen)
    {
        // Open the dialog to select a scene file with the optional preselected scene to open.
        std::string file;
        if (!sceneToOpen.empty())
            file = FileDialogs::OpenFile(Constants::FileType::SceneFiles, nullptr, sceneToOpen.c_str());
        else
            if (m_ActiveSceneFilePath.empty())
                file = FileDialogs::OpenFile(Constants::FileType::SceneFiles);
            else
                file = FileDialogs::OpenFile(Constants::FileType::SceneFiles, nullptr, m_ActiveSceneFilePath.c_str());
        
        if (file.empty())
        {
            PKE_CORE_WARN("Invalid scene file.");
            return false;
        }
        
        m_ActiveScene->EmptyScene();
        
        SceneSerializer(m_ActiveScene).Deserialize(file);
        m_ActiveSceneFilePath = file;
        return true;
    }

    bool EditorContext::SaveScene()
    {
        if (m_ActiveSceneFilePath.empty())
        {
            PKE_CORE_WARN("No active scene file path. Redirecting to Save As.");
            return SaveSceneAs();
        }
        
        SceneSerializer(m_ActiveScene).Serialize(m_ActiveSceneFilePath);
        return true;
    }

    bool EditorContext::SaveSceneAs(const std::string& newSceneName)
    {
        std::string file;
        if (!m_ActiveSceneFilePath.empty())
            file = FileDialogs::SaveFile(Constants::FileType::SceneFiles, m_ActiveSceneFilePath.c_str(), newSceneName.c_str());
        else
            file = FileDialogs::SaveFile(Constants::FileType::SceneFiles, nullptr, newSceneName.c_str());
        
        if (file.empty())
        {
            PKE_CORE_WARN("Invalid scene file.");
            return false;
        }
        
        m_ActiveSceneFilePath = file;
        
        SceneSerializer(m_ActiveScene).Serialize(file);
        return true;
    }
}
