file(GLOB_RECURSE sources ./src *.cpp)

set(UTILS_HEADERS_PATH ../SGMutils/inc)

add_executable(SGMsink ${sources})

include_directories(SGMsink ./inc ${UTILS_HEADERS_PATH} ${MQTT_HEADERS_PATH})

set_target_properties(SGMsink PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SGM_DEPLOY_BINARY_OUTPUT_PATH})

target_link_libraries(SGMsink pthread systemd paho-mqtt3as mqttpp rt SGMutils)
