/**
 * @file button_event.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "button_event.h"
#include <stdint.h>

static void updateInternalState(struct event_state *self, bool is_pressed,
                                uint64_t timestamp) {
  self->previous_state = is_pressed;
  self->previous_timestamp = timestamp;
}

bool push_event_update(struct event_state *self, bool is_pressed,
                       uint64_t timestamp) {
  uint32_t time_interval = timestamp - self->previous_timestamp;
  if (time_interval > UINT16_MAX)
    time_interval = UINT16_MAX;

  bool is_released = self->previous_state > is_pressed;
  updateInternalState(self, is_pressed, timestamp);
  if (is_released)
    return false;

  if (time_interval < self->debounce)
    return false;

  return true;
}

bool release_event_update(struct event_state *self, bool is_pressed,
                          uint64_t timestamp) {
  uint32_t time_interval = timestamp - self->previous_timestamp;
  if (time_interval > UINT16_MAX)
    time_interval = UINT16_MAX;

  bool is_released = self->previous_state > is_pressed;
  updateInternalState(self, is_pressed, timestamp);
  if (!is_released)
    return false;

  if (time_interval < self->debounce)
    return false;

  return true;
}

bool click_event_update(struct event_state *self, bool is_pressed,
                        uint64_t timestamp) {
  uint32_t time_interval = timestamp - self->previous_timestamp;
  if (time_interval > UINT16_MAX)
    time_interval = UINT16_MAX;

  bool is_released = self->previous_state > is_pressed;
  if (is_released || (!is_released && (timestamp >= self->debounce)))
    updateInternalState(self, is_pressed, timestamp);

  if (!is_released)
    return false;

  return true;
}

bool d_click_event_update(struct event_state *self, bool is_pressed,
                          uint64_t timestamp) {
  static bool clicked_once = false;

  uint32_t time_interval = timestamp - self->previous_timestamp;
  if (time_interval > UINT16_MAX)
    time_interval = UINT16_MAX;

  bool is_released = self->previous_state > is_pressed;
  if (is_released ||
      (!is_released && (timestamp >= self->debounce) && !clicked_once) ||
      (!is_released && clicked_once) ||
      (!is_released && (timestamp <= self->event_length_ms) && clicked_once))
    updateInternalState(self, is_pressed, timestamp);

  if (!is_released) {
    if (timestamp > self->event_length_ms)
      clicked_once = false;
    return false;
  }

  if (!clicked_once) {
    clicked_once = true;
    return false;
  }

  clicked_once = false;

  return true;
}

bool long_press_event_update(struct event_state *self, bool is_pressed,
                             uint64_t timestamp) {
  uint32_t time_interval = timestamp - self->previous_timestamp;
  if (time_interval > UINT16_MAX)
    time_interval = UINT16_MAX;

  bool is_released = self->previous_state > is_pressed;
  if (is_released || (!is_released && (timestamp >= self->debounce)))
    updateInternalState(self, is_pressed, timestamp);

  if (!is_released)
    return false;

  if (time_interval < self->event_length_ms)
    return false;

  return true;
}
