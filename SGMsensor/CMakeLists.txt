set(BIN_NAME SGMsensor)

set(BME280_SRC_PATH "${PROJECT_SOURCE_DIR}/3rdparty/BME280_driver")

file(GLOB_RECURSE sensor_sources ./src *.c??)
set (bme280_sources ${BME280_SRC_PATH}/bme280.c)

set(UTILS_HEADERS_PATH ../SGMutils/inc)

add_executable(${BIN_NAME} ${bme280_sources} ${sensor_sources})

include_directories(${BIN_NAME} ./inc ${UTILS_HEADERS_PATH} ${MQTT_HEADERS_PATH} ${BME280_SRC_PATH})

set_target_properties(${BIN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SGM_DEPLOY_BINARY_OUTPUT_PATH})

target_link_libraries(${BIN_NAME} pthread systemd rt mraa SGMutils)

file(GLOB SINK_SERVICE_FILES ./${BIN_NAME}.service/*)

file(COPY ${SINK_SERVICE_FILES} DESTINATION  ${SGM_DEPLOY_SERVICES_OUTPUT_PATH})