project "ImGui"
    location "imgui"
    kind "StaticLib"
    language "C++"

    -- The following is required to export the ImGui functions when building as a shared library
    defines { "IMGUI_API=__declspec(dllexport)" }

    targetdir ("bin/" .. outputdir .. "/%{prjName}")
    objdir ("bin-int/" .. outputdir .. "/%{prjName}")

    projectFolder = "imgui"

    files
    {
        projectFolder.."/*.h",
        projectFolder.."/*.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++20"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "speed"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"


project "GLFW"
    location "GLFW"
    kind "StaticLib"
    language "C"
    warnings "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    projectFolder = "GLFW"

    files
    {
        projectFolder.."/include/GLFW/glfw3.h",
        projectFolder.."/include/GLFW/glfw3native.h",
        projectFolder.."/src/context.c",
        projectFolder.."/src/init.c",
        projectFolder.."/src/input.c",
        projectFolder.."/src/monitor.c",
        projectFolder.."/src/null_init.c",
        projectFolder.."/src/null_joystick.c",
        projectFolder.."/src/null_monitor.c",
        projectFolder.."/src/null_window.c",
        projectFolder.."/src/platform.c",
        projectFolder.."/src/vulkan.c",
        projectFolder.."/src/window.c",
    }
    
    filter "system:linux"
        pic "On"
        
        systemversion "latest"
        
        files
        {
            projectFolder.."/src/x11_init.c",
            projectFolder.."/src/x11_monitor.c",
            projectFolder.."/src/x11_window.c",
            projectFolder.."/src/xkb_unicode.c",
            projectFolder.."/src/posix_module.c",
            projectFolder.."/src/posix_time.c",
            projectFolder.."/src/posix_thread.c",
            projectFolder.."/src/posix_module.c",
            projectFolder.."/src/glx_context.c",
            projectFolder.."/src/egl_context.c",
            projectFolder.."/src/osmesa_context.c",
            projectFolder.."/src/linux_joystick.c"
        }
        
        defines
        {
            "_GLFW_X11"
        }
    
    filter "system:macosx"
        pic "On"
        
        files
        {
            projectFolder.."/src/cocoa_init.m",
            projectFolder.."/src/cocoa_monitor.m",
            projectFolder.."/src/cocoa_window.m",
            projectFolder.."/src/cocoa_joystick.m",
            projectFolder.."/src/cocoa_time.c",
            projectFolder.."/src/nsgl_context.m",
            projectFolder.."/src/posix_thread.c",
            projectFolder.."/src/posix_module.c",
            projectFolder.."/src/osmesa_context.c",
            projectFolder.."/src/egl_context.c"
        }
    
        defines
        {
            "_GLFW_COCOA"
        }
    
    filter "system:windows"
        systemversion "latest"
    
        files
        {
            projectFolder.."/src/win32_init.c",
            projectFolder.."/src/win32_joystick.c",
            projectFolder.."/src/win32_module.c",
            projectFolder.."/src/win32_monitor.c",
            projectFolder.."/src/win32_time.c",
            projectFolder.."/src/win32_thread.c",
            projectFolder.."/src/win32_window.c",
            projectFolder.."/src/wgl_context.c",
            projectFolder.."/src/egl_context.c",
            projectFolder.."/src/osmesa_context.c"
        }
        
        defines
        {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "speed"
    
    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"


project "Glad"
    location "glad"
	kind "StaticLib"
	language "C"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    projectFolder = "glad"

	files
	{
		projectFolder.."/include/glad/glad.h",
		projectFolder.."/include/KHR/khrplatform.h",
        projectFolder.."/src/glad.c"
	}

	includedirs
	{
        projectFolder.."/include"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

	filter "system:macosx"
		pic "On"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
