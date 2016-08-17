cmake_minimum_required(VERSION 2.8.11)

set(EDISON_SDK /opt/poky-edison/1.7.2/)
set(EDISON_TOOLCHAIN ${EDISON_SDK}/sysroots/x86_64-pokysdk-linux)

set(CMAKE_C_COMPILER ${EDISON_TOOLCHAIN}/usr/bin/i586-poky-linux/i586-poky-linux-gcc)
set(CMAKE_CXX_COMPILER ${EDISON_TOOLCHAIN}/usr/bin/i586-poky-linux/i586-poky-linux-g++)

set(CMAKE_SYSROOT ${EDISON_SDK}/sysroots/core2-32-poky-linux)

project(SGMeteo C CXX)

#Set the target parameters
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 3.10.17-poky-edison+)
set(CMAKE_SYSTEM_PROCESSOR i686)

#disabling in-source build
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#set root path search options
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT} ${EDISON_TOOLCHAIN})

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#shared common compilation options
set (TARGET_ARCH "-m32 -march=i586")
set (COMPILER_OPTIONS "-Wall -fmessage-length=0 -ffunction-sections -fdata-sections --sysroot=${CMAKE_SYSROOT}")

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

set(MQTT_HEADERS_PATH ../../paho.mqtt.cpp/src/)

include_directories(${CMAKE_SYSROOT}/usr/include)
include_directories(${CMAKE_SYSROOT}/usr/include/c++)
include_directories(${CMAKE_SYSROOT}/usr/include/c++/i586-poky-linux)

add_subdirectory(./SGMutils)
add_subdirectory(./SGMsink)
add_subdirectory(./SGMsensor)
