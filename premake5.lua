--/FS
workspace "test_refactor_edit_control_02"
    platforms { "64" }
    configurations { "Debug", "Release" }
	architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "json_reader"
	location "json_reader"
	kind "StaticLib"
	language "C++"
	staticruntime "Off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines {
		"__lib_build"
	}

	files {
		"%{prj.location}/**.c",
		"%{prj.location}/**.cpp",
		"%{prj.location}/**.h",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.json"
	}

	links {
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib";
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib"
	}

	
	filter "system:windows"
		cppdialect "Default"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"
		defines {
			"__DEBUG"
		}

	filter "configurations:Release"
		optimize "On"
		runtime "Release"
		defines {
			"__RELEASE"
		}

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
        "%{prj.location}/*.rc",
        "%{prj.location}/*.h",
        "%{prj.location}/*.cpp",
        "%{prj.location}/src/**"
    }
    
    links {
        "kernel32.lib",
        "user32.lib",
        "gdi32.lib";
        "winspool.lib",
        "comdlg32.lib",
        "advapi32.lib",
        "shell32.lib",
        "ole32.lib",
        "oleaut32.lib",
        "uuid.lib",
        "odbc32.lib",
        "odbccp32.lib"
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
