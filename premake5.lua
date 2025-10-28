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
	["ImGui"] = "Jah/External/imgui",
	["ImGuiBackends"] = "Jah/External/imgui/backends",
	["glm"] = "Jah/External/glm",
	["stb_image"] = "Jah/External/stb_image",
	["spdlog"] = "Jah/External/spdlog/include",
}

group "Dependencies"
	include "Jah/External/GLFW"
	include "Jah/External/Glad"
	include "Jah/External/imgui"
group ""

project "Jah"
	location "Jah"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	defines {
		"GLFW_INCLUDE_NONE",
	}

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/External/stb_image/**.h",
		"%{prj.name}/External/stb_image/**.cpp",
		"%{prj.name}/External/glm/glm/**.hpp",
		"%{prj.name}/External/glm/glm/**.inl",
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	includedirs {
		"%{prj.name}/Source/",
		IncludeDirs.GLFW,
		IncludeDirs.Glad,
		IncludeDirs.ImGui,
		IncludeDirs.ImGuiBackends,
		IncludeDirs.glm,
		IncludeDirs.stb_image,
		IncludeDirs.spdlog,
	}

	pchheader "jahpch.h"
	pchsource "Jah/Source/jahpch.cpp"

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
		buildoptions { "/utf-8" }

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
        "%{wks.location}/Jah/Source",
		"%{wks.location}/Jah/External",
		IncludeDirs.ImGui,
		IncludeDirs.glm,
		IncludeDirs.Glad,
		IncludeDirs.spdlog,
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

project "Jah-Editor"
	location "Jah-Editor"
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
        "%{wks.location}/Jah/Source",
		"%{wks.location}/Jah/External",
		IncludeDirs.ImGui,
		IncludeDirs.glm,
		IncludeDirs.Glad,
		IncludeDirs.spdlog,
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

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }
