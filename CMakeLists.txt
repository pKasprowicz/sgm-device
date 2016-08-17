cmake_minimum_required(VERSION 3.6)

project(SGMeteo C CXX)

set(EDISON_SDK_PATH /opt/poky-edison/1.6)
set(EDISON_SYSROOT_PATH ${EDISON_SDK_PATH}/sysroots/core2-32-poky-linux)
set(EDISON_TOOLCHAIN_PATH ${EDISON_SDK_PATH}/sysroots/x86_64-pokysdk-linux)

set (CMAKE_SYSROOT ${EDISON_SYSROOT_PATH})

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set (CMAKE_CXX_COMPILER ${EDISON_TOOLCHAIN_PATH}/usr/bin/i586-poky-linux/i586-poky-linux-g++)
set (CMAKE_CXX_COMPILER ${EDISON_TOOLCHAIN_PATH}/usr/bin/i586-poky-linux/i586-poky-linux-gcc)

INCLUDE_DIRECTORIES(${CMAKE_SYSROOT}/usr/include)
INCLUDE_DIRECTORIES(${CMAKE_SYSROOT}/usr/include/c++)
INCLUDE_DIRECTORIES(${CMAKE_SYSROOT}/usr/include/c++/i586-poky-linux)

SET(CMAKE_C_FLAGS "-Os -g3 -Wall --sysroot=${CMAKE_SYSROOT} -m32 -march=i586 -ffunction-sections -fdata-sections"  CACHE STRING "" FORCE)
SET(CMAKE_CXX_FLAGS "-std=c++0x -Os -g3 -Wall --sysroot=${CMAKE_SYSROOT} -m32 -march=i586 -ffunction-sections -fdata-sections"  CACHE STRING "" FORCE)

add_subdirectory(SGMutils)