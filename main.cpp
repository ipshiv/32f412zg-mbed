/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include <array>

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms

#define GREEN PE_0
#define ORANGE PE_1
#define RED PE_2
#define BLUE PE_3

int main()
{
	// Initialise the digital pin LED1 as an output
	DigitalOut led_green(GREEN);
	DigitalOut led_orange(ORANGE);
	DigitalOut led_red(RED);
	DigitalOut led_blue(BLUE);
	std::array<DigitalOut, 4> leds = {
		led_green, led_orange, led_red, led_blue
	};

	while (true) {
		for (auto &led : leds) {
			led.write(0);
			ThisThread::sleep_for(BLINKING_RATE);
			led.write(1);
		}
	}
}
