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

InterruptIn b_down(DOWN);
InterruptIn b_left(LEFT);
InterruptIn b_right(RIGHT);

static auto blinking_rate = 500ms;

Timer t;

Queue<uint32_t, 32> event_queue;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread queue_thread;
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

void button_handler(int id, bool is_pressed, uint64_t timestamp) {
  printf(">> @ %llu Button %d %s\n", timestamp, id,
         is_pressed ? "pressed" : "released");
  event_queue.try_put((uint32_t *)id);
}

void log_up(int evt) { printf(">>UP button >> evt %d\n", evt); }

void log_down() {
  queue.call(button_handler, 1, true,
             duration_cast<milliseconds>(t.elapsed_time()).count());
}

void log_left() {
  queue.call(button_handler, 2, true,
             duration_cast<milliseconds>(t.elapsed_time()).count());
}

void log_right() {
  queue.call(button_handler, 3, true,
             duration_cast<milliseconds>(t.elapsed_time()).count());
}

int main() {
  // b_up.rise(&log_up);
  b_down.rise(&log_down);
  b_left.rise(&log_left);
  b_right.rise(&log_right);

  t.start();
  ButtonHandler handler;
  printf("attach button");
  handler.attachButton(UP, &log_up);
  handler.registerEvent(UP, BUTTON_EVENT_PUSH, 200);
  // handler.registerEvent(UP, BUTTON_EVENT_RELEASED, 200);

  queue_thread.start(callback(&queue, &EventQueue::dispatch_forever));
  led_thread.start(callback(led_handler));
}
