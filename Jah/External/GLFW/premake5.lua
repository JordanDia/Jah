project "GLFW"
	location "Jah/External/GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.c",
    	"include/**.h"
    }

	filter "system:windows"
		buildoptions { "-std=c11", "-lgdi32" }
		systemversion "latest"
		staticruntime "On"
		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"