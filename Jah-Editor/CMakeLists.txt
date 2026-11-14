file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    Source/*.h
    Source/*.cpp
)

add_executable(Jah-Editor ${EDITOR_SOURCES})
target_link_libraries(Jah-Editor PRIVATE Jah)