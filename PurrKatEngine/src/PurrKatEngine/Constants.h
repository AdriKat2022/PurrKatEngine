#pragma once

namespace PurrKatEngine
{
    class Constants
    {
    public:
        // Scenes
        static constexpr const char* SCENE_EXTENSION = ".pkscene";
        static constexpr const char* DEFAULT_SCENE_NAME = "Untitled";
        static constexpr const char* NO_ACTIVE_SCENE_NAME = "Untitled*";
        
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
