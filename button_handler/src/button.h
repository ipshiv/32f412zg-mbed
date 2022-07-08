/**
 * @file button.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _BUTTON_H
#define _BUTTON_H

#include "button_event.h"
#include "button_events.h"
#include "mbed.h"
#include <stdbool.h>
#include <stdint.h>
#include <vector>

using namespace std::chrono;

// TODO Refactor to compilator variable
#define MAX_EVENTS_PER_BUTTON 4

class Button {
public:
  Button(PinName id, EventQueue *queue, Timer *t, Callback<void(int)> cb,
         Callback<void()> handler) {
    pin = new InterruptIn(id);
    pin->rise(handler);
    pin->fall(handler);
    state = pin->read();
    self_id = id;
    queue = queue;
    t = t;
    cb = cb;
  }

  uint32_t id(void) { return (uint32_t)self_id; }

  void checkButton() {
    if (events.empty())
      return;

    if (pin->read() != state) {
      pin->disable_irq();

      state = pin->read();
      uint64_t timestamp =
          duration_cast<milliseconds>(t->elapsed_time()).count();
      for (auto &event : events) {
        bool is_emmited = event->updateButtonState(state, timestamp);
        if (is_emmited)
          queue->call(cb, event->type());
      }

      pin->enable_irq();
    }
  }

  void buttonPressed(void) {
    if (events.empty())
      return;
    uint64_t timestamp = duration_cast<milliseconds>(t->elapsed_time()).count();
    for (auto &event : events) {
      bool is_emmited = event->updateButtonState(true, timestamp);
      if (is_emmited)
        queue->call(cb, event->type());
    }
  }

  void buttonReleased(void) {
    if (events.empty())
      return;
    uint64_t timestamp = duration_cast<milliseconds>(t->elapsed_time()).count();
    for (auto &event : events) {
      bool is_emmited = event->updateButtonState(false, timestamp);
      if (is_emmited)
        queue->call(cb, event->type());
    }
  }

  bool registerEvent(enum button_event evt_type, uint16_t event_length_ms,
                     uint16_t debounce) {
    if (events.size() == MAX_EVENTS_PER_BUTTON)
      return false;

    ButtonEvent *new_event;
    switch (evt_type) {
    case (BUTTON_EVENT_PUSH):
      new_event = new PushEvent(evt_type, event_length_ms, debounce);
      break;
    case (BUTTON_EVENT_RELEASED):
      new_event = new ReleaseEvent(evt_type, event_length_ms, debounce);
      break;
    case (BUTTON_EVENT_CLICK):
      new_event = new ClickEvent(evt_type, event_length_ms, debounce);
      break;
    case (BUTTON_EVENT_DOUBLE_CLICK):
      new_event = new DoubleClickEvent(evt_type, event_length_ms, debounce);
      break;
    case (BUTTON_EVENT_LONG_PRESS):
      new_event = new LongPress(evt_type, event_length_ms, debounce);
      break;
    default:
      return false;
    }

    if (new_event == nullptr)
      return false;

    events.push_back(new_event);

    return true;
  }

private:
  std::vector<ButtonEvent *> events;
  EventQueue *queue;
  Callback<void(int)> cb;
  Timer *t;
  PinName self_id;
  InterruptIn *pin;
  bool state;
};

#endif // _BUTTON_H
