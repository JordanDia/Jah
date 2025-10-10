project "Glad"
	location "Jah/External/Glad"
	kind "StaticLib"
	language "C"

	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

	files {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c",
    }

    includedirs {
        "include"
    }

	filter "system:windows"
		systemversion "latest"
		buildoptions { "-std=c11", "-lgdi32" }
		staticruntime "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"