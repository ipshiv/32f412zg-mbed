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
#include <memory>
#include <stdbool.h>
#include <stdint.h>
#include <vector>

using namespace std::chrono;

// TODO Refactor to compilator variable
#define MAX_EVENTS_PER_BUTTON 4

class Button {
public:
  Button() = delete;
  Button(PinName id, EventQueue *queue, Timer *t, void (*cb)(int),
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

  void attachISRHandler(Callback<void()> cb) {
    this->pin->rise(cb);
    this->pin->fall(cb);
  }

  void checkButton(uint64_t timestamp) {
    if (events.empty())
      return;

    if (pin->read() != state) {
      state = pin->read();

      printf(">> Button %d, State: %s\n", self_id, state ? "UP" : "DOWN");

      for (auto &event : events) {
        bool is_emmited = event->updateButtonState(state, timestamp);
        printf(">>  Button %d >> Event %d >> Emmited: %s\n", self_id,
               event->type(), is_emmited ? "true" : "false");
      }
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
      new_event = (new PushEvent(evt_type, event_length_ms, debounce));
      break;
    case (BUTTON_EVENT_RELEASED):
      new_event = (new ReleaseEvent(evt_type, event_length_ms, debounce));
      break;
    case (BUTTON_EVENT_CLICK):
      new_event = (new ClickEvent(evt_type, event_length_ms, debounce));
      break;
    case (BUTTON_EVENT_DOUBLE_CLICK):
      new_event = (new DoubleClickEvent(evt_type, event_length_ms, debounce));
      break;
    case (BUTTON_EVENT_LONG_PRESS):
      new_event = (new LongPress(evt_type, event_length_ms, debounce));
      break;
    default:
      return false;
    }

    printf("Registered event %d\n", new_event->type());

    events.push_back(new_event);

    return true;
  }

private:
  std::vector<ButtonEvent *> events;
  EventQueue *queue;
  void (*cb)(int);
  Timer *t;
  PinName self_id;
  InterruptIn *pin;
  bool state;
};

#endif // _BUTTON_H
