#include "EditorContext.h"

#include "PurrKatEngine/Constants.h"
#include "PurrKatEngine/PlatformUtils.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Serialization/SceneSerializer.h"
#include "PurrKatEngine/Utility/FileUtility.h"

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
        if (m_ActiveSceneFilePath.empty()) return {};
        
        return FileUtility::GetFileNameWithoutExtension(m_ActiveSceneFilePath);
    }

    void EditorContext::NewScene()
    {
        m_ActiveSceneFilePath.clear();
        m_ActiveScene->EmptyScene();
    }

    /**
     * Prompts the user to select a scene file to open. If a scene file path is provided, it will bypass the dialog prompt and open that scene directly.
     * @param sceneToOpen Scene to open (if provided, bypasses the dialog prompt).
     * @return Whether the operation was completed (true) or cancelled/failed (false).
     */
    bool EditorContext::OpenScene(const std::string& sceneToOpen)
    {
        // Open the dialog to select a scene file with the optional preselected scene to open.
        std::string file;
        if (sceneToOpen.empty())
        {
            if (m_ActiveSceneFilePath.empty())
                file = FileDialogs::OpenFile(Constants::FileType::SceneFiles);
            else
                file = FileDialogs::OpenFile(Constants::FileType::SceneFiles, m_ActiveSceneFilePath.c_str());
        }
        else
            file = sceneToOpen;

        if (file.empty())
        {
            PKE_CORE_WARN("Invalid scene file (sceneToOpen='{}').", sceneToOpen);
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
