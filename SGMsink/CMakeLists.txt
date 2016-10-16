set(BIN_NAME SGMsink)

file(GLOB_RECURSE sources ./src *.cpp)

set(UTILS_HEADERS_PATH ../SGMutils/inc)

add_executable(${BIN_NAME} ${sources})

include_directories(${BIN_NAME} ./inc ${UTILS_HEADERS_PATH} ${MQTT_HEADERS_PATH})

set_target_properties(${BIN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SGM_DEPLOY_BINARY_OUTPUT_PATH})

target_link_libraries(${BIN_NAME} pthread systemd paho-mqtt3as mqttpp rt SGMutils)

file(COPY ./SGMsink.service/SGMsink.service ./SGMsink.service/SGMsink.socket DESTINATION ${SGM_DEPLOY_SERVICES_OUTPUT_PATH})