workspace "PurrKatEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    staticruntime "on"
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder of solution
IncludeDirs = {}
IncludeDirs["GLFW"] = "PurrKatEngine/vendor/GLFW/include"
IncludeDirs["Glad"] = "PurrKatEngine/vendor/glad/include"
IncludeDirs["ImGui"] = "PurrKatEngine/vendor/imgui"
IncludeDirs["Glm"] = "PurrKatEngine/vendor/glm"
IncludeDirs["stb_image"] = "PurrKatEngine/vendor/stb_image"

group "Dependencies"
    include "PurrKatEngine/vendor"
group ""

project "PurrKatEngine"
    location "PurrKatEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pkepch.h"
    pchsource "PurrKatEngine/src/pkepch.cpp"

    defines { "_CRT_SECURE_NO_WARNINGS" }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "%{prj.location}/vendor/spdlog/include",
        "%{prj.location}/src",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.ImGui}",
        "%{IncludeDirs.Glm}",
        "%{IncludeDirs.stb_image}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "PKE_PLATFORM_WINDOWS",
            "PKE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            '{MKDIR} "../bin/' .. outputdir .. '/Sandbox"',
            '{COPY} "%{cfg.buildtarget.relpath}" "../bin/' .. outputdir .. '/Sandbox"'
        }

    filter { "configurations:Debug" }
        defines { "PKE_DEBUG", "PKE_ENABLE_ASSERTS" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        runtime "Release"
        optimize "on"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        runtime "Release"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

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
        "%{wks.location}/PurrKatEngine/vendor",
        "%{wks.location}/PurrKatEngine/src",
        "%{IncludeDirs.Glm}",
    }

    links
    {
        "PurrKatEngine",
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "PKE_PLATFORM_WINDOWS"
        }

    filter { "configurations:Debug" }
        defines { "PKE_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        defines { "PKE_RELEASE" }
        runtime "Release"
        optimize "on"

    filter { "configurations:Dist" }
        defines { "PKE_DIST" }
        runtime "Release"
        optimize "on"