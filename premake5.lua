workspace "PurrKatEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder of solution
IncludeDirs = {}
IncludeDirs["GLFW"] = "PurrKatEngine/vendor/GLFW/include"

include "PurrKatEngine/vendor/GLFW"

project "PurrKatEngine"
    location "PurrKatEngine"
    kind "SharedLib"
    language "C++"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pkepch.h"
    pchsource "PurrKatEngine/src/pkepch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/vendor/spdlog/include",
        "%{prj.location}/src",
        "%{IncludeDirs.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PKE_PLATFORM_WINDOWS",
            "PKE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter { "configurations:Debug" }
        defines { "PKE_DEBUG", "PKE_ENABLE_ASSERTS" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        optimize "On"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/PurrKatEngine/vendor/spdlog/include",
        "%{wks.location}/PurrKatEngine/src"
    }

    links
    {
        "PurrKatEngine"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PKE_PLATFORM_WINDOWS"
        }

    filter { "configurations:Debug" }
        defines { "PKE_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        optimize "On"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        optimize "On"