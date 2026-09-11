#pragma once
#include <filesystem>
#include "imgui.h"
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/ImGui/Components/ItemBrowser.h"
#include "PurrKatEngine/Utility/EventAction.h"

namespace PurrKatEngine
{
    class Texture2D;

    class ContentBrowserPanel
    {
    public:
        EventAction<std::string> OnSceneOpenRequest;
        
    public:
        ContentBrowserPanel();
        
        void OnImGuiRender();
        void ScheduleNavigatePath(const std::string& folderNameToNavigate);
        void NavigatePath(const std::string& folderNameToNavigate);
        void NavigatePathPrevious();
        void RefreshDirectory();
        
    private:
        void HandleItemClick(const ElementItem* item, bool isDoubleClick) const;

    private:
        std::filesystem::path m_CurrentDirectory;
        std::vector<std::filesystem::directory_entry> m_Entries;
        
        ItemBrowser m_ItemBrowser{
            .Debugging = true,
            .DefaultThumbnail = nullptr,
            .OnClick = [this](const ElementItem* item) { HandleItemClick(item, false); },
            .OnDoubleClick = [this](const ElementItem* item) { HandleItemClick(item, true); }
        };
        
        float m_ThumbnailSize = 40;
        bool m_GridView = true;
        
        float m_ItemPadding = 5;
        
        Ref<Texture2D> m_FolderIconTex;
        ImTextureRef m_FolderIcon;
    };
}
