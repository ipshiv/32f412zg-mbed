/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "rtos.h"
#include <array>
#include <cstdint>
#include <stdint.h>

#include "button_event.h"
#include "button_handler.h"

using namespace std::chrono;

#define GREEN LED1
#define ORANGE LED2
#define RED LED3
#define BLUE LED4
#define UP JOY_UP
#define DOWN JOY_DOWN
#define LEFT JOY_LEFT
#define RIGHT JOY_RIGHT



static auto blinking_rate = 500ms;

Queue<uint32_t, 32> event_queue;
Thread led_thread;

void led_handler(void) {
  printf(">> Leds Init!\n");

  int active_led = 0;

  DigitalOut led_green(GREEN);
  DigitalOut led_orange(ORANGE);
  DigitalOut led_red(RED);
  DigitalOut led_blue(BLUE);

  std::array<DigitalOut *, 4> leds = {&led_green, &led_orange, &led_red,
                                      &led_blue};

  while (true) {
    osEvent evt = event_queue.get();
    if (evt.status == osEventMessage) {
      leds[active_led]->write(0);
      active_led = evt.value.v;
    }
    leds[active_led]->write(0);
    ThisThread::sleep_for(blinking_rate);
    leds[active_led]->write(1);
    ThisThread::sleep_for(blinking_rate);
  }
}

void log_up(int evt) { printf(">>UP button >> evt %d\n", evt); }

int main() {
  ButtonHandler handler;
  printf("Attach buttons to handler\n");

  handler.attachButton(UP, &log_up);

  handler.registerEvent(UP, BUTTON_EVENT_PUSH, 200);

  handler.attachButton(LEFT, &log_up);
  handler.registerEvent(LEFT, BUTTON_EVENT_RELEASED, 200);

  handler.attachButton(DOWN, &log_up);
  handler.registerEvent(DOWN, BUTTON_EVENT_CLICK, 200);

  handler.attachButton(RIGHT, &log_up);
  handler.registerEvent(RIGHT, BUTTON_EVENT_LONG_PRESS, 2000);

  led_thread.start(callback(led_handler));
}
