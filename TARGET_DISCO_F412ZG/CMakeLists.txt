# Copyright (c) 2022 Huld Oy. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-disco-f412zg INTERFACE)

target_sources(mbed-disco-f412zg
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-disco-f412zg
    INTERFACE
        .
)

target_link_libraries(mbed-disco-f412zg INTERFACE mbed-stm32f412xg)
