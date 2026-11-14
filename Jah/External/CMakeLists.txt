add_library(Glad STATIC Glad/src/glad.c)
target_include_directories(Glad PUBLIC Glad/include)

add_library (ImGui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(ImGui PUBLIC
    imgui
    imgui/backends
)
target_link_libraries(ImGui PRIVATE glfw)
add_library(ImGuizmo STATIC ImGuizmo/ImGuizmo.cpp)
target_include_directories(ImGuizmo PUBLIC ImGuizmo)
target_link_libraries(ImGuizmo PUBLIC ImGui)

# ---------------------------------------------------------------
# Part 2: libraries that ship their own CMakeLists.txt
# ---------------------------------------------------------------

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(GLFW)

# Box2D
add_subdirectory(Box2D)

# yaml-cpp
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(yaml-cpp)
target_compile_options(yaml-cpp PRIVATE /wd4267)

#FreeType
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
add_subdirectory(freetype)

# ---------------------------------------------------------------
# Part 3: header-only libraries (INTERFACE targets)
# ---------------------------------------------------------------

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE spdlog/include)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb_image stb_truetype)

add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE miniaudio)

# ---------------------------------------------------------------
# Part 4: prebuilt libraries (IMPORTED targets)
# ---------------------------------------------------------------

add_library(mono STATIC IMPORTED GLOBAL)
set_target_properties(mono PROPERTIES
    IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/mono/lib/Debug/libmono-static-sgen.lib
    IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/mono/lib/Release/libmono-static-sgen.lib
    IMPORTED_LOCATION_DIST ${CMAKE_CURRENT_SOURCE_DIR}/mono/lib/Dist/libmono-static-sgen.lib
)
target_include_directories(mono INTERFACE mono/include)
target_link_libraries(mono INTERFACE Ws2_32 Winmm Version Bcrypt)