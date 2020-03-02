--/FS
workspace "test_refactor_edit_control_02"
	startproject "test_refactor_edit_control_02"

    platforms { "64" }
    configurations { "Debug", "Release" }
	architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "test_refactor_edit_control_02"
    location "test_refactor_edit_control_02"
    kind "WindowedApp"
    language "C++"
    staticruntime "Off"

    postbuildcommands {
        "{COPY} %{prj.location}bkg_image.bmp %{cfg.targetdir}"
    }

    pchheader "pch.h"
    pchsource "%{prj.location}/src/edit_control/_precompiled_headers/pch.cpp"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "%{prj.location}/src",
        "%{prj.location}/src/edit_control/_precompiled_headers"
    }

    files {
        "%{prj.location}/**"
    }
    
    links {
        "json_reader"
    }

    
    filter "system:windows"
        cppdialect "Default"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "On"
        runtime "Release"
        

project "json_reader"
	location "json_reader"
	kind "StaticLib"
	language "C++"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines {}

	files {
		"%{prj.location}/**"
	}

	
	filter "system:windows"
		cppdialect "Default"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"