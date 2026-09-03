#include "pkepch.h"
#include "FileUtility.h"

namespace PurrKatEngine
{
    std::string FileUtility::GetFileName(const std::string& filePath)
    {
        if (filePath.empty()) return {};
        
        size_t lastSlashPos = filePath.find_last_of("/\\");
        return filePath.substr(lastSlashPos + 1);
    }

    std::string FileUtility::GetFileNameWithoutExtension(const std::string& filePath)
    {
        if (filePath.empty()) return {};
        
        size_t lastSlashPos = filePath.find_last_of("/\\");
        size_t lastDotPos = filePath.find_last_of('.');
        
        if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos)
            return filePath.substr(lastSlashPos + 1); // No extension found, return the full filename.
        
        return filePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
    }

    std::string FileUtility::GetFileExtension(const std::string& filePath)
    {
        if (filePath.empty()) return {};
        
        size_t lastSlashPos = filePath.find_last_of("/\\");
        size_t lastDotPos = filePath.find_last_of('.');
        
        if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos)
            return {}; // No extension found.
        
        return filePath.substr(lastDotPos + 1);
    }
}
