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
	["GLFW"] = "Jah/External/GLFW/include"
}

project "GLFW"
	location "Jah/External/GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	files {
        "Jah/External/GLFW/src/**.c"
    }

    includedirs {
        "Jah/External/GLFW/include"
    }

	filter "system:windows"
		systemversion "latest"
		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
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

project "Jah"
	location "Jah"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	links {
		"GLFW",
		"opengl32.lib"
	}

	includedirs {
		"%{prj.name}/Source/",
		IncludeDirs.GLFW,
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