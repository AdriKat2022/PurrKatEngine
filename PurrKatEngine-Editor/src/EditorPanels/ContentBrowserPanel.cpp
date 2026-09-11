#include "pkepch.h"
#include "ContentBrowserPanel.h"

#include <filesystem>
#include <string>
#include "imgui.h"
#include "PurrKatEngine/Constants.h"
#include "PurrKatEngine/ImGui/Components/ItemBrowser.h"
#include "PurrKatEngine/Logs/InternalLog.h"
#include "PurrKatEngine/Renderer/Texture.h"

namespace PurrKatEngine
{
    constexpr const char* s_AssetsDirectory = "assets";

    static bool scheduledPreviousPathFlag = false;
    static bool scheduledNextPathFlag = false;
    static std::string scheduledNextPath;
    
    ContentBrowserPanel::ContentBrowserPanel()
        : m_CurrentDirectory(s_AssetsDirectory)
    {
        m_FolderIconTex = Texture2D::CreateRef("assets/textures/folder.png", { .Filter = Texture2D::FilterType::Nearest });
        m_FolderIcon = m_FolderIconTex->GetRendererID();
        
        RefreshDirectory();
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        m_ItemBrowser.Draw("Content Browser", nullptr);
        
        if (scheduledNextPathFlag)
        {
            NavigatePath(scheduledNextPath);
            scheduledNextPathFlag = false;
        }
        
        if (scheduledPreviousPathFlag)
        {
            NavigatePathPrevious();
            scheduledPreviousPathFlag = false;
        }
    }

    void ContentBrowserPanel::ScheduleNavigatePath(const std::string& folderNameToNavigate)
    {
        scheduledNextPathFlag = true;
        scheduledNextPath = folderNameToNavigate;
    }
    
    void ContentBrowserPanel::NavigatePath(const std::string& folderNameToNavigate)
    {
        m_CurrentDirectory /= folderNameToNavigate;
        RefreshDirectory();
    }
    
    void ContentBrowserPanel::NavigatePathPrevious()
    {
        if (m_CurrentDirectory == s_AssetsDirectory)
            return;
        
        m_CurrentDirectory = m_CurrentDirectory.parent_path();
        RefreshDirectory();
    }
    
    void ContentBrowserPanel::RefreshDirectory()
    {
        m_ItemBrowser.ClearItems();
        m_Entries.clear();
        
        if (m_CurrentDirectory != s_AssetsDirectory)
        {
            m_ItemBrowser.AddItem({
                .Type = -1,
                .Name = "..",
                .ThumbnailTexture = m_FolderIcon
            });
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            m_Entries.push_back(entry);
            ElementItem item = {
                .Type = entry.is_directory() ? 0 : 1,
                .Name = entry.path().filename().string(),
                .ThumbnailTexture = entry.is_directory() ? m_FolderIcon : nullptr
            };
            m_ItemBrowser.AddItem(item);
        }
    }
    
    void ContentBrowserPanel::HandleItemClick(const ElementItem* item, bool isDoubleClick) const
    {
        PKE_CORE_DEBUG("Item clicked: ID={}, Type={}, Label={}", item->Id, item->Type, item->Name);
        
        if (item->Type == -1) // Folder
        {
            scheduledPreviousPathFlag = true;
        }
        else if (item->Type == 0) // Folder
        {
            scheduledNextPathFlag = true;
            scheduledNextPath = item->Name;
        }
        else if (item->Type == 1 && isDoubleClick) // File
        {
            // Handle file click (e.g., open or import)
            if (item->Name.ends_with(Constants::SCENE_EXTENSION))
            {
                OnSceneOpenRequest.Invoke((m_CurrentDirectory / item->Name).string());
            }
            else if (item->Name.ends_with(".png") || item->Name.ends_with(".jpg") || item->Name.ends_with(".jpeg"))
            {
                PKE_CORE_INFO("Image file clicked: {}", item->Name);
                // Implement image opening logic here
            }
            else if (item->Name.ends_with(".fbx") || item->Name.ends_with(".obj"))
            {
                PKE_CORE_INFO("3D model file clicked: {}", item->Name);
                // Implement 3D model opening logic here
            }
            else
            {
                PKE_CORE_WARN("File type not supported for: {}", item->Name);
            }
        }
    }
}
