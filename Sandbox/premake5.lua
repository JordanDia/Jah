project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
    staticruntime "Off"

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

