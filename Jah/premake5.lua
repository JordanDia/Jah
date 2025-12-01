project "Jah"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "Off"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	pchheader "jahpch.h"
	pchsource "Source/jahpch.cpp"
	pchsource "Source/jahpch.cpp"

	files {
		"Source/**.h",
		"Source/**.cpp",
		"External/stb_image/**.h",
		"External/stb_image/**.cpp",
		"External/glm/glm/**.hpp",
		"External/glm/glm/**.inl",

		"External/ImGuizmo/ImGuizmo.h",
		"External/ImGuizmo/ImGuizmo.cpp",
	}

	defines {
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE",
	}

	includedirs {
		"Source",
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
		IncludeDirs.mono,
	}

	links {
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",

		Library.mono,
	}

	filter "files:External/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

		links {
			Library.WinSock,
			Library.WinMM,
			Library.WinVersion,
			Library.BCrypt,
		}

	filter "configurations:Debug"
		defines { "JAH_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "JAH_RELEASE" }
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines { "JAH_DIST" }
		optimize "On"
		runtime "Release"