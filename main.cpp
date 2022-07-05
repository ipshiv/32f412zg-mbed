/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "rtos.h"
#include <cstdint>
#include <stdint.h>
#include <array>

#define GREEN LED1
#define ORANGE LED2
#define RED LED3
#define BLUE LED4
#define UP JOY_UP
#define DOWN JOY_DOWN
#define LEFT JOY_LEFT
#define RIGHT JOY_RIGHT

InterruptIn b_up(UP);
InterruptIn b_down(DOWN);
InterruptIn b_left(LEFT);
InterruptIn b_right(RIGHT);

DigitalOut led_green(GREEN);
DigitalOut led_orange(ORANGE);
DigitalOut led_red(RED);
DigitalOut led_blue(BLUE);

static int led_id = 0;

static auto blinking_rate = 500ms;

Queue<uint32_t, 16> event_queue;

void button_handler(void)
{
	printf(">> Buttons!\n");
	while (true) {
		osEvent evt = event_queue.get();
		if (evt.status != osEventMessage) {
			printf("queue->get() returned %02x status\n\r",
			       evt.status);
		} else {
			printf("queue->get() returned %d\n\r", evt.value.v);
		}
		ThisThread::sleep_for(1s);
	}
}

void log_up()
{
	// printf(">> Up!\n");
	event_queue.put((uint32_t *)0);
}

void log_down()
{
	// printf(">> Down!\n");
	event_queue.put((uint32_t *)1);
}

void log_left()
{
	// printf(">> Left\n");
	event_queue.put((uint32_t *)2);
}

void log_right()
{
	// printf(">> Right\n");
	event_queue.put((uint32_t *)3);
}

int main()
{
	b_up.rise(&log_up);
	b_down.rise(&log_down);
	b_left.rise(&log_left);
	b_right.rise(&log_right);

	std::array<DigitalOut, 4> leds = {
		led_green, led_orange, led_red, led_blue
	};

	Thread thread;
	thread.start(callback(button_handler));

	while (true) {
		for (auto &led : leds) {
			led.write(0);
			ThisThread::sleep_for(blinking_rate);
			led.write(1);
		}
	}
}
