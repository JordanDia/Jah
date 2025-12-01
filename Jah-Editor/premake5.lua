project "Jah-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
    staticruntime "Off"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/**.h",
        "Source/**.cpp",
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

	