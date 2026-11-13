project "freetype"
    kind "StaticLib"
    language "C"
    staticruntime "Off"
    systemversion "latest"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Binaries-Int/" .. outputdir .. "/%{prj.name}")

    -- Minimal sources: TTF rendering only
    files {
        "src/autofit/**.c",
        "src/base/**.c",
        "src/truetype/**.c",
        "src/raster/**.c",
        "src/sfnt/**.c",
        "src/smooth/**.c",
        "src/winfonts/**.c",
        "src/type42/**.c",
        "src/type1/**.c",
        "src/psnames/**.c",
        "src/pshinter/**.c",
        "src/psaux/**.c",
        "src/pfr/**.c",
        "src/pcf/**.c",
        "src/svg/**.c",
        "src/sdf/**.c",
        "src/cff/**.c",
        "src/bdf/**.c",

        "src/**/**.c",
    }

    includedirs {
        "include"  -- FreeType public headers
    }

    defines {
        "FT2_BUILD_LIBRARY",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
