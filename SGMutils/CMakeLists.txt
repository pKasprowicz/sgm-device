file(GLOB_RECURSE sources ./src/*.c*)

add_library(SGMutils STATIC ${sources})

target_include_directories(SGMutils PUBLIC inc)
target_link_libraries(SGMutils mraa)