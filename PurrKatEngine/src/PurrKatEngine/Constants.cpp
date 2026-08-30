#include "pkepch.h"
#include "Constants.h"

namespace PurrKatEngine
{
    // doesn't want to initialize, to check
     const std::unordered_map<Constants::FileType, const Constants::FileFilterType> Constants::FileFilters = {
        {FileType::AllFiles, {.Extension = "*.*", .Description = "All Files (*.*)"}},
        {FileType::TextFiles, {.Extension = "*.txt", .Description = "Text Files (*.txt)"}},
        {FileType::ImageFiles, {.Extension = "*.png;*.jpg;*.jpeg;*.bmp;*.gif", .Description = "Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.gif)"}},
        {FileType::CppFiles, {.Extension = "*.cpp;*.h",     .Description = "C++ Source Files (*.cpp;*.h)"}},
        {FileType::SceneFiles, {.Extension = "*.pkscene",   .Description = "PurrKat Scene (*.pkscene)"}}
    };
}
