file(GLOB_RECURSE sources ./src *.cpp)

add_library(SGMutils STATIC ${sources})

include_directories(SGMutils ./inc)

set_target_properties(SGMutils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/libs)

target_link_libraries(SGMutils)
