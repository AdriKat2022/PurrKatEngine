#pragma once

namespace PurrKatEngine
{
    class FileUtility
    {
    public:
        static std::string GetFileName(const std::string& filePath);
        static std::string GetFileNameWithoutExtension(const std::string& filePath);
        static std::string GetFileExtension(const std::string& filePath);
    };
}
