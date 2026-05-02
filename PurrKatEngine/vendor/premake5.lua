project "ImGui"
    location "imgui"
    kind "StaticLib"
    language "C++"

    targetdir ("imgui/bin/" .. outputdir .. "/%{prjName}")
    objdir ("imgui/bin-int/" .. outputdir .. "/%{prjName}")

    files
    {
        "imgui/*.h",
        "imgui/*.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"
        staticruntime "off"
