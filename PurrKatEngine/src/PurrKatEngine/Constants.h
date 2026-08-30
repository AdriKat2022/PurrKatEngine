#pragma once

namespace PurrKatEngine
{
    class Constants
    {
    public:
        // Scenes
        static constexpr const char* DefaultSceneName = "Untitled";
        
        // File Types
        enum class FileType
        {
            AllFiles,
            TextFiles,
            ImageFiles,
            CppFiles,
            SceneFiles,
        };
        
        
        struct FileFilterType
        {
            const char* Extension;
            const char* Description;
        };
        
        static const std::unordered_map<FileType, const FileFilterType> FileFilters;
    };
}
