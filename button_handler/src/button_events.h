/**
 * @file button_events.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _BUTTON_EVENTS_H
#define _BUTTON_EVENTS_H

#include "button_event.h"
#include <array>
#include <stdbool.h>
#include <stdint.h>

class ButtonEvent {
public:
  ButtonEvent(enum button_event evt_type, uint16_t event_length_ms,
              uint16_t debounce) {
    this->evt_type = evt_type;
    this->event_length_ms = event_length_ms;
    this->debounce = debounce;
    this->previous_state = false;
    this->previous_timestamp = 0;
  }
  enum button_event type(void) { return this->evt_type; }
  virtual bool updateButtonState(bool is_pressed, uint64_t timestamp) = 0;

protected:
  void updateInternalState(bool is_pressed, uint64_t timestamp) {
    this->previous_state = is_pressed;
    this->previous_timestamp = timestamp;
  }
  enum button_event evt_type;
  uint16_t event_length_ms;
  uint16_t debounce;
  bool previous_state;
  uint64_t previous_timestamp;
};

class PushEvent : public ButtonEvent {
public:
  PushEvent(enum button_event evt_type, uint16_t event_length_ms,
            uint16_t debounce)
      : ButtonEvent(evt_type, event_length_ms, debounce){};
  bool updateButtonState(bool is_pressed, uint64_t timestamp) {
    uint32_t time_interval = timestamp - previous_timestamp;
    if (time_interval > UINT16_MAX)
      time_interval = UINT16_MAX;

    bool is_released = previous_state > is_pressed;
    updateInternalState(is_pressed, timestamp);
    if (is_released)
      return false;

    if (time_interval < debounce)
      return false;

    return true;
  }
};

class ReleaseEvent : public ButtonEvent {
public:
  ReleaseEvent(enum button_event evt_type, uint16_t event_length_ms,
               uint16_t debounce)
      : ButtonEvent(evt_type, event_length_ms, debounce){};
  bool updateButtonState(bool is_pressed, uint64_t timestamp) {
    uint32_t time_interval = timestamp - previous_timestamp;
    if (time_interval > UINT16_MAX)
      time_interval = UINT16_MAX;

    bool is_released = previous_state > is_pressed;
    updateInternalState(is_pressed, timestamp);
    if (!is_released)
      return false;

    if (time_interval < debounce)
      return false;

    return true;
  }
};

class ClickEvent : public ButtonEvent {
public:
  ClickEvent(enum button_event evt_type, uint16_t event_length_ms,
             uint16_t debounce)
      : ButtonEvent(evt_type, event_length_ms, debounce){};
  /**
   * @brief
   *
   * @param is_pressed
   * @param timestamp
   * @return true
   * @return false
   *
   * @note:
   * 		debounce				debounce
   *     _________			  	_________
   * 				|			 	|
   * 				|	__________ 	|
   */
  bool updateButtonState(bool is_pressed, uint64_t timestamp) {
    uint32_t time_interval = timestamp - previous_timestamp;
    if (time_interval > UINT16_MAX)
      time_interval = UINT16_MAX;

    bool is_released = previous_state > is_pressed;
    if (is_released || (!is_released && (timestamp >= debounce)))
      updateInternalState(is_pressed, timestamp);

    if (!is_released)
      return false;

    return true;
  }
};

class DoubleClickEvent : public ButtonEvent {
public:
  DoubleClickEvent(enum button_event evt_type, uint16_t event_length_ms,
                   uint16_t debounce)
      : ButtonEvent(evt_type, event_length_ms, debounce){};
  /**
   * @brief
   *
   * @param is_pressed
   * @param timestamp
   * @return true
   * @return false
   *
   * @note:
   * 		debounce				evt_length
   * debounce
   *     _________			  	_________
   * _________ |			 	|		|
   * | |	__________ 	|		|	__________ 	|
   *
   */
  bool updateButtonState(bool is_pressed, uint64_t timestamp) {
    uint32_t time_interval = timestamp - previous_timestamp;
    if (time_interval > UINT16_MAX)
      time_interval = UINT16_MAX;

    bool is_released = previous_state > is_pressed;
    if (is_released ||
        (!is_released && (timestamp >= debounce) && !clicked_once) ||
        (!is_released && clicked_once) ||
        (!is_released && (timestamp <= event_length_ms) && clicked_once))
      updateInternalState(is_pressed, timestamp);

    if (!is_released) {
      if (timestamp > event_length_ms)
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

private:
  bool clicked_once;
};

class LongPress : public ButtonEvent {
public:
  LongPress(enum button_event evt_type, uint16_t event_length_ms,
            uint16_t debounce)
      : ButtonEvent(evt_type, event_length_ms, debounce){};
  /**
   * @brief
   *
   * @param is_pressed
   * @param timestamp
   * @return true
   * @return false
   *
   * @note:
   * 		debounce	evt_length	debounce
   *     _________			  	_________
   * 				|			 	|
   * 				|	__________ 	|
   */
  bool updateButtonState(bool is_pressed, uint64_t timestamp) {
    uint32_t time_interval = timestamp - previous_timestamp;
    if (time_interval > UINT16_MAX)
      time_interval = UINT16_MAX;

    bool is_released = previous_state > is_pressed;
    if (is_released || (!is_released && (timestamp >= debounce)))
      updateInternalState(is_pressed, timestamp);

    if (!is_released)
      return false;

    if (time_interval < event_length_ms)
      return false;

    return true;
  }
};

#endif // _BUTTON_EVENTS_H
