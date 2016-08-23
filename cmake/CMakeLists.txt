set(MQTTPP_BASE_DIR "../../org.eclipse.paho.mqtt.cpp")

set(MQTTPP_SRC "${MQTTPP_BASE_DIR}/src")
set(MQTTPP_INC "${MQTTPP_SRC}")

set(TARGET_NAME mqttpp)

aux_source_directory(${MQTTPP_SRC} MQTTPP_SOURCES)

add_library(mqttpp SHARED ${MQTTPP_SOURCES})

include_directories(${TARGET_NAME} ${MQTTPP_INC})

set_target_properties(${TARGET_NAME} PROPERTIES
                                LIBRARY_OUTPUT_DIRECTORY ${SGM_DEPLOY_SHARED_OUTPUT_PATH}
                                SOVERSION 0
                                VERSION 1.0.0)

target_link_libraries(${TARGET_NAME} c stdc++ pthread)