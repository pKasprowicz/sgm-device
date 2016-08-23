cmake_minimum_required(VERSION 2.8.11)

set(EDISON_SDK /opt/poky-edison/1.7.3)
set(EDISON_TOOLCHAIN ${EDISON_SDK}/sysroots/x86_64-pokysdk-linux)

set(CMAKE_C_COMPILER ${EDISON_TOOLCHAIN}/usr/bin/i586-poky-linux/i586-poky-linux-gcc)
set(CMAKE_CXX_COMPILER ${EDISON_TOOLCHAIN}/usr/bin/i586-poky-linux/i586-poky-linux-g++)

set(COMPILER_SYSROOT ${EDISON_SDK}/sysroots/core2-32-poky-linux)

#############################################
# Deployment directory structure definition #
#############################################

set(SGM_DEPLOY_BIN_PATH /home/root/sgm)
set(SGM_DEPLOY_SHARED_PATH /usr/lib)
set(SGM_DEPLOY_SERVICES_PATH /etc/systemd/system)

set(SGM_DEPLOY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/out/poky)


set(SGM_DEPLOY_LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out/libs)
set(SGM_DEPLOY_BINARY_OUTPUT_PATH ${SGM_DEPLOY_OUTPUT_DIR}/${SGM_DEPLOY_BIN_PATH})
set(SGM_DEPLOY_SHARED_OUTPUT_PATH ${SGM_DEPLOY_OUTPUT_DIR}/${SGM_DEPLOY_SHARED_PATH})

set(SGM_DEPLOY_SERVICES_OUTPUT_PATH ${SGM_DEPLOY_OUTPUT_DIR}/${SGM_DEPLOY_SERVICES_PATH})

###################
# Project section #
###################

set(PROJECT_NAME SGMeteo)

project(${PROJECT_NAME} C CXX)

#Set the target parameters
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 3.10.17-poky-edison+)
set(CMAKE_SYSTEM_PROCESSOR i686)

#disabling in-source build
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#set root path search options
set(CMAKE_FIND_ROOT_PATH ${EDISON_TOOLCHAIN})

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#shared common compilation options
set (TARGET_ARCH "-m32 -march=i586")
set (COMPILER_OPTIONS "-Wall -fmessage-length=0 -ffunction-sections -fdata-sections --sysroot=${COMPILER_SYSROOT}")

#shared C compilation options
set (C_LANGUAGE_DIALECT "-std=c99")

#shared C++ compilation options
set (CXX_LANGUAGE_DIALECT "-std=c++0x")

#release config compilation options
set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${COMPILER_OPTIONS} ${TARGET_ARCH} ${CXX_LANGUAGE_DIALECT}")
set(CMAKE_C_FLAGS_RELEASE "-O2 ${COMPILER_OPTIONS} ${TARGET_ARCH} ${C_LANGUAGE_DIALECT}")

#debug config compilation option
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ${COMPILER_OPTIONS} ${TARGET_ARCH} ${CXX_LANGUAGE_DIALECT}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g ${COMPILER_OPTIONS} ${TARGET_ARCH} ${C_LANGUAGE_DIALECT}")

set(MQTT_HEADERS_PATH ../../org.eclipse.paho.mqtt.cpp/src/)

message("-- Checking for MQTT CPP library sources: ${MQTT_HEADERS_PATH}")

if(NOT EXISTS ${MQTT_HEADERS_PATH})
    message("--  MQTT CPP library sources not found in ${MQTT_HEADERS_PATH}")
    message("Attempting to fetch MQTT CPP sources...")

    execute_process(
    COMMAND "git" "clone" "http://git.eclipse.org/gitroot/paho/org.eclipse.paho.mqtt.cpp.git"
    WORKING_DIRECTORY "../../"
    )
else(NOT EXISTS ${MQTT_HEADERS_PATH})
    message("-- Checking for MQTT CPP library sources: ${MQTT_HEADERS_PATH} -- OK")
endif(NOT EXISTS ${MQTT_HEADERS_PATH})

include_directories(${COMPILER_SYSROOT}/usr/include)
include_directories(${COMPILER_SYSROOT}/usr/include/c++)
include_directories(${COMPILER_SYSROOT}/usr/include/c++/i586-poky-linux)

add_subdirectory(./cmake)
add_subdirectory(./SGMutils)
add_subdirectory(./SGMsink)
add_subdirectory(./SGMsensor)
add_subdirectory(./SGMpowermgr)

add_custom_target(
    installer
    ALL
    COMMAND "makeself" "./out/poky" ./out/install.sh SGMeteo echo "Done"
    DEPENDS SGMpowermgr mqttpp SGMutils SGMsink SGMsensor
    WORKING_DIRECTORY ./
    VERBATIM
)