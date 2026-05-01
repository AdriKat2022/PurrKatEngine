workspace "PurrKatEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder of solution
IncludeDirs = {}
IncludeDirs["GLFW"] = "PurrKatEngine/vendor/GLFW/include"
IncludeDirs["Glad"] = "PurrKatEngine/vendor/glad/include"
IncludeDirs["ImGui"] = "PurrKatEngine/vendor/imgui"

include "PurrKatEngine/vendor/GLFW"
include "PurrKatEngine/vendor/glad"
include "PurrKatEngine/vendor/imgui"

project "PurrKatEngine"
    location "PurrKatEngine"
    kind "SharedLib"
    language "C++"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pkepch.h"
    pchsource "PurrKatEngine/src/pkepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/vendor/spdlog/include",
        "%{prj.location}/src",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.ImGui}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PKE_PLATFORM_WINDOWS",
            "PKE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter { "configurations:Debug" }
        defines { "PKE_DEBUG", "PKE_ENABLE_ASSERTS" }
        buildoptions "/MDd"
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        buildoptions "/MD"
        optimize "On"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        buildoptions "/MD"
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
        buildoptions "/MDd"
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        buildoptions "/MD"
        optimize "On"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        buildoptions "/MD"
        optimize "On"