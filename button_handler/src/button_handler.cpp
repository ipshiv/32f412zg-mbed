/**
 * @file button_handler.cpp
 * @author Ivan Shivarev (ivan.shivarev@huld.io)
 * @brief
 * @version 0.1
 * @date 2022-07-05
 *
 * @copyright Copyright (c) 2022 Huld Oy
 *
 */

#include "button_handler.h"
#include "button.h"
#include "mbed.h"
#include <vector>

#define MAX_BUTTONS 4

Timer button_timer;
EventQueue button_queue(32 * EVENTS_EVENT_SIZE);
Thread button_thread;

std::vector<Button> buttons;

void b_checker(uint64_t timestamp) {

  for (auto &b : buttons) {
    b.checkButton(timestamp);
  }
}

void isr_handler(void) {

  uint64_t timestamp =
      duration_cast<milliseconds>(button_timer.elapsed_time()).count();
  button_queue.call(b_checker, timestamp);
}

ButtonHandler::ButtonHandler() {
  button_timer.start();
  button_thread.start(callback(&button_queue, &EventQueue::dispatch_forever));
}

int ButtonHandler::attachButton(uint32_t button, void (*func)(int)) {

  if (buttons.size() == MAX_BUTTONS)
    return -1;

  for (auto &b : buttons) {
    if (b.id() == button) {
      return -1;
    }
  }

  Button *new_button = new Button((PinName)button, &button_queue, &button_timer,
                                  func, isr_handler);
  new_button->attachISRHandler(isr_handler);
  buttons.push_back(*new_button);
  printf("Attached %ld\n", new_button->id());
  return 0;
}

int ButtonHandler::registerEvent(uint32_t button, enum button_event evt_type,
                                 uint16_t evt_time) {
  if (buttons.empty())
    return -1;

  for (auto &b : buttons) {
    if (b.id() == button) {
      if (b.registerEvent(evt_type, evt_time, 200)) {
        printf("Button %ld >> Event %ld >> Registered\n", b.id(), evt_type);
        return 0;
      }
      break;
    }
  }

  return -1;
}
