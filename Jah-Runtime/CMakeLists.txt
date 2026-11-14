file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDSa
    Source/*.h
    Source/*.cpp
)

add_executable(Jah-Runtime ${RUNTIME_SOURCES})
target_link_libraries(Jah-Runtime PRIVATE Jah)

set_target_properties(Jah-Runtime PROPERTIES WIN32_EXECUTABLE $<CONFIG:Dist>)
target_link_options(Jah-Runtime PRIVATE $<$<CONFIG:Dist>:ENTRY:mainCTRStartup>)