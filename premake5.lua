workspace "Jah"
	architecture "x64"
	startproject "Jah-Editor"

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
	["yaml_cpp"] = "Jah/External/yaml-cpp/include",
	["ImGuizmo"] = "Jah/External/ImGuizmo",
	["Box2D"] = "Jah/External/Box2D/include",
}

group "Dependencies"
	include "Jah/External/GLFW"
	include "Jah/External/Glad"
	include "Jah/External/imgui"
	include "Jah/External/yaml-cpp"
	include "Jah/External/Box2D"
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
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
	}

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/External/stb_image/**.h",
		"%{prj.name}/External/stb_image/**.cpp",
		"%{prj.name}/External/glm/glm/**.hpp",
		"%{prj.name}/External/glm/glm/**.inl",
		"%{prj.name}/External/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/External/ImGuizmo/ImGuizmo.cpp",
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"Box2D",
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
		IncludeDirs.yaml_cpp,
		IncludeDirs.ImGuizmo,
		IncludeDirs.Box2D,
	}

	pchheader "jahpch.h"
	pchsource "Jah/Source/jahpch.cpp"

	filter "files:Jah/External/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

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

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

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
		"%{wks.location}/Jah/External/ImGuizmo/ImGuizmo.h",
		"%{wks.location}/Jah/External/ImGuizmo/ImGuizmo.cpp",
    }

    includedirs {
        "%{wks.location}/Jah/Source",
		"%{wks.location}/Jah/External",
		IncludeDirs.ImGui,
		IncludeDirs.glm,
		IncludeDirs.Glad,
		IncludeDirs.spdlog,
		IncludeDirs.ImGuizmo,
		IncludeDirs.Box2D,
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
