set(BIN_NAME SGMutils)

file(GLOB_RECURSE sources ./src *.cpp)

add_library(${BIN_NAME} STATIC ${sources})

include_directories(${BIN_NAME} ./inc)

set_target_properties(${BIN_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SGM_DEPLOY_LIBRARY_OUTPUT_PATH})

target_link_libraries(${BIN_NAME})
