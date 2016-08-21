set(BME280_SRC_PATH "${PROJECT_SOURCE_DIR}/3rdparty/BME280_driver")

file(GLOB_RECURSE sensor_sources ./src *.c??)
set (bme280_sources ${BME280_SRC_PATH}/bme280.c)

set(UTILS_HEADERS_PATH ../SGMutils/inc)

add_executable(SGMsensor ${bme280_sources} ${sensor_sources})

include_directories(SGMsensor ./inc ${UTILS_HEADERS_PATH} ${MQTT_HEADERS_PATH} ${BME280_SRC_PATH})

set_target_properties(SGMsensor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SGM_DEPLOY_BINARY_OUTPUT_PATH})

target_link_libraries(SGMsensor pthread systemd rt mraa SGMutils)
