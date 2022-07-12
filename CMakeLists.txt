# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET mbed-os-example-blinky)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(TARGET_DISCO_F412ZG)
add_subdirectory(${MBED_PATH})
#add_subdirectory(button_handler)

add_executable(${APP_TARGET}
    main.cpp
    button_handler/src/button_handler.cpp
    button_handler/src/button_event.c
)

target_include_directories(${APP_TARGET}
PRIVATE
    button_handler/include
)

target_link_libraries(${APP_TARGET}
        mbed-os
        mbed-events
        )

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
