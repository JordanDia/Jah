project "Jah-Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++23"
    staticruntime "Off"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    files {
		"Source/**.h",
        "Source/**.cpp",
    }

    includedirs {
        "%{wks.location}/Jah/Source",
		"%{wks.location}/Jah/External",
		IncludeDirs.Box2D,
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
		optimize "Full"
		runtime "Release"
		entrypoint "mainCRTStartup"
		linkoptions { "/SUBSYSTEM:WINDOWS" }
		
		postbuildcommands {
			-- Copy VC runtime DLLs (adjust paths if needed)
			"{COPY} C:/Windows/System32/vcruntime140.dll %{cfg.targetdir}",
			"{COPY} C:/Windows/System32/vcruntime140_1.dll %{cfg.targetdir}",
			"{COPY} C:/Windows/System32/msvcp140.dll %{cfg.targetdir}",

			-- Copy mono runtime DLL
			"{COPY} %{wks.location}/Jah-Runtime/mono/ %{cfg.targetdir}/mono",

			-- Copy Assets, Resources
			"{COPY} %{wks.location}/Jah-Runtime/Assets %{cfg.targetdir}/Assets",
			"{COPY} %{wks.location}/Jah-Runtime/Resources %{cfg.targetdir}/Resources",

			-- Copy imgui.ini
			"{COPY} %{wks.location}/Jah-Runtime/imgui.ini %{cfg.targetdir}",
		}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

