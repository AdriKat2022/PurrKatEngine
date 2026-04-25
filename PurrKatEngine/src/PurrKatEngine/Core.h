#pragma once

#ifdef PKE_PLATFORM_WINDOWS
    #ifdef PKE_BUILD_DLL
        #define PKE_API __declspec(dllexport)
    #else
        #define PKE_API __declspec(dllimport)
    #endif
#else
    #error MyGameEngine only supports windows for now!
#endif

