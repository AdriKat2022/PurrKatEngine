workspace "PurrKatEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PurrKatEngine"
    location "PurrKatEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include"
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
        defines { "PKE_DEBUG" }
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

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "PurrKatEngine/vendor/spdlog/include",
        "PurrKatEngine/src"
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