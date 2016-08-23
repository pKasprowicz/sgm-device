set(BIN_NAME SGMpowermgr)

file(GLOB_RECURSE powermgr_sources ./src *.c??)

set(UTILS_HEADERS_PATH ../SGMutils/inc)

add_executable(${BIN_NAME} ${powermgr_sources})

include_directories(${BIN_NAME} ./inc ${UTILS_HEADERS_PATH} ${MQTT_HEADERS_PATH} ${BME280_SRC_PATH})

set_target_properties(${BIN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SGM_DEPLOY_BINARY_OUTPUT_PATH})

target_link_libraries(${BIN_NAME} pthread systemd rt mraa SGMutils)

file(GLOB SINK_SERVICE_FILES ./${BIN_NAME}.service/*)

file(COPY ${SINK_SERVICE_FILES} DESTINATION  ${SGM_DEPLOY_SERVICES_OUTPUT_PATH})