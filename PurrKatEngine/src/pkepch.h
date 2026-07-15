#pragma once

#include <algorithm>
#include <array>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "glm/glm.hpp"
#include "PurrKatEngine/Core.h"
#include "PurrKatEngine/Utility/Utility.h"
#include "PurrKatEngine/Utility/Mathf.h"

#ifdef PKE_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif
