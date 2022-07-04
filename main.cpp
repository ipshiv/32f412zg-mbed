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
#define UP PG_0
#define DOWN PG_1
#define LEFT PF_15
#define RIGHT PF_14

InterruptIn b_up(UP);
InterruptIn b_down(DOWN);
InterruptIn b_left(LEFT);
InterruptIn b_right(RIGHT);

DigitalOut led_green(GREEN);
DigitalOut led_orange(ORANGE);
DigitalOut led_red(RED);
DigitalOut led_blue(BLUE);

static int led_id = 0;

static int blinking_rate = 200;

void next_led()
{
	blinking_rate += 100;
}

void prev_led()
{
	blinking_rate -= 100;
}

int main()
{
	b_up.rise(&next_led);
	b_down.rise(&prev_led);
	b_left.rise(&next_led);
	b_right.rise(&prev_led);

	std::array<DigitalOut, 4> leds = {
		led_green, led_orange, led_red, led_blue
	};

	while (true) {
		for (auto &led : leds) {
			led.write(0);
			ThisThread::sleep_for(blinking_rate);
			led.write(1);
		}
	}
}
