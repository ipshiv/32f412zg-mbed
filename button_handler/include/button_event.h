/**
 * @file button_event.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _BUTTON_EVENT_H
#define _BUTTON_EVENT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

enum button_event {
  BUTTON_EVENT_DISABLED = 0,
  BUTTON_EVENT_PUSH = 1,
  BUTTON_EVENT_RELEASED = 2,
  BUTTON_EVENT_CLICK = 3,
  BUTTON_EVENT_DOUBLE_CLICK = 4,
  BUTTON_EVENT_LONG_PRESS = 5,
};

struct event_state {
  enum button_event type;
  uint16_t event_length_ms;
  uint16_t debounce;
  bool previous_state;
  uint64_t previous_timestamp;
};

struct event {
  struct event_state state;
  bool (*update_state)(struct event_state *self, bool is_pressed,
                       uint64_t timestamp);
};

bool push_event_update(struct event_state *self, bool is_pressed,
                       uint64_t timestamp);

bool release_event_update(struct event_state *self, bool is_pressed,
                          uint64_t timestamp);

bool click_event_update(struct event_state *self, bool is_pressed,
                        uint64_t timestamp);

bool d_click_event_update(struct event_state *self, bool is_pressed,
                          uint64_t timestamp);

bool long_press_event_update(struct event_state *self, bool is_pressed,
                             uint64_t timestamp);

#ifdef __cplusplus
}
#endif
#endif // _BUTTON_EVENT_H
