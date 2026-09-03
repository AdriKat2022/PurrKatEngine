#include "ContentBrowserPanel.h"

#include <filesystem>
#include <string>
#include "imgui.h"

namespace PurrKatEngine
{
    constexpr const char* s_AssetsDirectory = "assets";

    ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory(s_AssetsDirectory) {}

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");
        
        ImGui::SetWindowFontScale(1.2f);
        
        ImGui::Text("Viewing directory: \"%s\"", m_CurrentDirectory.string().c_str());
        
        if (m_CurrentDirectory != s_AssetsDirectory && ImGui::Button("<-"))
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        
        std::filesystem::directory_iterator dirIter(m_CurrentDirectory);
        
        for (const std::filesystem::directory_entry& entry : dirIter)
        {
            std::string path = entry.path().filename().string();
            if (entry.is_directory())
            {
                if (ImGui::Button(path.c_str()))
                {
                    m_CurrentDirectory /= path;
                }
            }
            else
            {
                ImGui::Text("%s", path.c_str());
            }
        }
        ImGui::End();
    }
}
