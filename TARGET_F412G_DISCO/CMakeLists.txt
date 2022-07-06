# Copyright (c) 2022 Huld Oy. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-f412g-disco INTERFACE)

target_sources(mbed-f412g-disco
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-f412g-disco
    INTERFACE
        .
)

target_link_libraries(mbed-f412g-disco INTERFACE mbed-stm32f412xg)
