project "Jah-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Jah-Editor/Resources/Scripts")
    objdir ("%{wks.location}/Jah-Editor/Resources/Scripts/Intermediates")

    files {
        "Source/**.cs",
        "Properties/**.cs",
    }

	filter "configurations:Debug"
		defines { "JAH_DEBUG" }
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		defines { "JAH_RELEASE" }
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		defines { "JAH_DIST" }
		optimize "Full"
		symbols "Off"