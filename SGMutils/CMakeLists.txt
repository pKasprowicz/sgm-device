file(GLOB_RECURSE sources ./src *.cpp)

add_library(SGMutils STATIC ${sources})

include_directories(SGMutils ./inc)

set_target_properties(SGMutils PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SGM_DEPLOY_LIBRARY_OUTPUT_PATH})

target_link_libraries(SGMutils)
