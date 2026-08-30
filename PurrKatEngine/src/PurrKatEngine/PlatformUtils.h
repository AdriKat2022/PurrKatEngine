#pragma once

#include "PurrKatEngine/Constants.h"

namespace PurrKatEngine
{
    class FileDialogs
    {
    public:
        static std::string OpenFile(Constants::FileType filter = {}, const char* defaultPath = nullptr, const char* defaultFileName = nullptr);
        static std::string SaveFile(Constants::FileType filter = {}, const char* defaultPath = nullptr, const char* defaultFileName = nullptr);
    };
}
