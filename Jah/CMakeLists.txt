file(GLOB_RECURSE JAH_SOURCES CONFIGURE_DEPENDS
    Source/*.h
    Source/*.cpp
)

add_library(Jah STATIC
    ${JAH_SOURCES}
    External/stb_image/stb_image.cpp
    External/stb_truetype/stb_truetype.cpp
)

target_precompile_headers(Jah PRIVATE Source/jahpch.h)

target_include_directories(Jah PUBLIC
    Source
    External
)

target_compile_definitions(Jah PUBLIC
    GLFW_INCLUDE_NONE
    _CRT_SECURE_NO_WARNINGS
    $<$<CONFIG:Debug>:JAH_DEBUG>
    $<$<CONFIG:Release>:JAH_RELEASE>
    $<$<CONFIG:Dist>:JAH_DIST>
)

target_compile_options(Jah PUBLIC /utf-8)

target_link_libraries(Jah PUBLIC
    glfw
    Glad
    ImGui
    ImGuizmo
    box2d
    yaml-cpp
    freetype
    glm
    spdlog
    stb
    miniaudio
    mono
    opengl32
)