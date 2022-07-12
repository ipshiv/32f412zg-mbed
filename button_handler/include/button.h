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
#include "mbed.h"
#include <stdbool.h>
#include <stdint.h>
#include <vector>

using namespace std::chrono;

// TODO Refactor to compilator variable
#define MAX_EVENTS_PER_BUTTON 4

class Button {
public:
  Button(PinName id, EventQueue *queue, Timer *t, void (*cb)(int),
         Callback<void()> handler) {
    this->pin = new InterruptIn(id);
    // pin->rise(buttonPressed);
    // pin->fall(buttonReleased);
    this->b_state = pin->read();
    this->self_id = id;
    this->queue = queue;
    this->t = t;
    this->cb = cb;
  }

  uint32_t id(void) { return (uint32_t)this->self_id; }

  void attachISRHandler(Callback<void()> cb) {
    this->pin->rise(cb);
    this->pin->fall(cb);
  }

  uint32_t checkButton(uint64_t timestamp) {

    // printf("Pressed\n");
    if (pin->read() != b_state) {
      // pin->disable_irq();

      this->b_state = pin->read();

      printf(">> Button %d, State: %s\n", self_id, b_state ? "UP" : "DOWN");
      uint64_t timestamp =
          duration_cast<milliseconds>(t->elapsed_time()).count();
      for (auto &event : events) {
        bool is_emmited = event.update_state(&event.state, b_state, timestamp);
        printf(">>  Button %d >> Event %d >> Emmited: %s\n", self_id,
               event.state.type, is_emmited ? "true" : "false");
        // if (is_emmited)
        //   return event.state.type;
      }

      // pin->enable_irq();
    }

    return 0;
  }

  // void buttonPressed(void)
  // {
  //   // if (!events.size())
  //   //   return;
  //   printf("Pressd\n");
  //   uint64_t timestamp =
  //   duration_cast<milliseconds>(t->elapsed_time()).count(); for (auto &event
  //   : events)
  //   {
  //     bool is_emmited = event->updateButtonState(true, timestamp);
  //     if (is_emmited)
  //       queue->call(cb, event->type());
  //   }
  // }

  // void buttonReleased(void)
  // {
  //   // if (!events.size())
  //   //   return;
  //   uint64_t timestamp =
  //   duration_cast<milliseconds>(t->elapsed_time()).count(); for (auto &event
  //   : events)
  //   {
  //     bool is_emmited = event->updateButtonState(false, timestamp);
  //     if (is_emmited)
  //       queue->call(cb, event->type());
  //   }
  // }

  bool registerEvent(enum button_event evt_type, uint16_t event_length_ms,
                     uint16_t debounce) {
    if (events.size() == MAX_EVENTS_PER_BUTTON)
      return false;

    struct event new_event;
    new_event.state.debounce = debounce;
    new_event.state.event_length_ms = event_length_ms;
    new_event.state.type = evt_type;
    new_event.state.previous_state = false;
    new_event.state.previous_timestamp = 0;

    // ButtonEvent *new_event;
    switch (evt_type) {
    case (BUTTON_EVENT_PUSH):
      new_event.update_state = push_event_update;
      break;
    case (BUTTON_EVENT_RELEASED):
      new_event.update_state = release_event_update;
      break;
    case (BUTTON_EVENT_CLICK):
      new_event.update_state = click_event_update;
      break;
    case (BUTTON_EVENT_DOUBLE_CLICK):
      new_event.update_state = d_click_event_update;
      break;
    case (BUTTON_EVENT_LONG_PRESS):
      new_event.update_state = long_press_event_update;
      break;
    default:
      return false;
    }

    events.push_back(new_event);
    printf("Now event size is %d\n", events.size());
    for (auto &event : events) {
      printf("Button %d >> Event %d >> Registered\n", self_id,
             event.state.type);
    }

    return true;
  }

private:
  std::vector<struct event> events;
  EventQueue *queue;
  void (*cb)(int);
  Timer *t;
  PinName self_id;
  InterruptIn *pin;
  bool b_state;
};

#endif // _BUTTON_H
