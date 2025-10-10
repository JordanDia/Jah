workspace "Jah"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags { "MultiProcessorCompile" }


outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDirs = {
	["GLFW"] = "Jah/External/GLFW/include",
	["Glad"] = "Jah/External/Glad/include",
}

include "Jah/External/GLFW"
include "Jah/External/Glad"

project "Jah"
	location "Jah"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	defines {
		"GLFW_INCLUDE_NONE"
	}

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	includedirs {
		"%{prj.name}/Source/",
		IncludeDirs.GLFW,
		IncludeDirs.Glad,
		"%{prj.name}/External/Glad/include",
	}

	filter "configurations:Debug"
		defines { "JAH_DEBUG" }
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines { "JAH_RELEASE" }
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines { "JAH_DIST" }
		optimize "On"
		runtime "Release"

	filter "system:windows"
		systemversion "latest"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
    staticruntime "on"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
    }

    includedirs {
        "Jah/Source",
    }

    links {
        "Jah"
    }

	filter "configurations:Debug"
		defines { "JAH_DEBUG" }
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines { "JAH_RELEASE" }
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines { "JAH_DIST" }
		optimize "On"
		runtime "Release"