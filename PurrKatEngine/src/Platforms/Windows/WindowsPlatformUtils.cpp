#include "pkepch.h"

#include "WindowsWindow.h"
#include "PurrKatEngine/Application.h"
#include "PurrKatEngine/PlatformUtils.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3native.h"

namespace PurrKatEngine
{
    std::string FileDialogs::OpenFile(Constants::FileType filter, const char* defaultPath, const char* defaultFileName)
    {
        Constants::FileFilterType fileFilterType = Constants::FileFilters.at(filter);
        
        std::string filterStr;
        filterStr.append(fileFilterType.Description);
        filterStr.push_back('\0');
        filterStr.append(fileFilterType.Extension);
        filterStr.push_back('\0');
        
        OPENFILENAMEA ofn = {};  // Initialize to zero
        CHAR szFile[260] = {};   // Buffer for file name

        // Initialize OPENFILENAME
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());

        // Set default file name (if provided)
        if (defaultFileName)
        {
            strncpy_s(szFile, defaultFileName, sizeof(szFile) - 1);
            // Ensure null-termination
            szFile[sizeof(szFile) - 1] = '\0';
        }

        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filterStr.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Set default directory (if provided)
        if (defaultPath)
        {
            ofn.lpstrInitialDir = defaultPath;
        }

        // Open the dialog
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            // Return the selected file path as a std::string
            return std::string(ofn.lpstrFile);
        }

        // Return empty string on cancel or error
        return {};
    }
    
    std::string FileDialogs::SaveFile(Constants::FileType filter, const char* defaultPath, const char* defaultFileName)
    {
        Constants::FileFilterType fileFilterType = Constants::FileFilters.at(filter);
        
        std::string filterStr;
        filterStr.append(fileFilterType.Description);
        filterStr.push_back('\0');
        filterStr.append(fileFilterType.Extension);
        filterStr.push_back('\0');
        
        OPENFILENAMEA ofn = {};  // Initialize to zero
        CHAR szFile[260] = {};   // Buffer for file name

        // Initialize OPENFILENAME
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());

        // Set default file name (if provided)
        if (defaultFileName)
        {
            strncpy_s(szFile, defaultFileName, sizeof(szFile) - 1);
            // Ensure null-termination
            szFile[sizeof(szFile) - 1] = '\0';
        }

        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filterStr.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Set default directory (if provided)
        if (defaultPath)
        {
            ofn.lpstrInitialDir = defaultPath;
        }

        // Open the dialog
        if (GetSaveFileNameA(&ofn) == TRUE) 
        {
            // Return the selected file path as a std::string
            return std::string(ofn.lpstrFile);
        }

        // Return empty string on cancel or error
        return {};
    }
}
