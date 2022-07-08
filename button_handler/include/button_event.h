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

enum button_event {
  BUTTON_EVENT_DISABLED = 0,
  BUTTON_EVENT_PUSH = 1,
  BUTTON_EVENT_RELEASED = 2,
  BUTTON_EVENT_CLICK = 3,
  BUTTON_EVENT_DOUBLE_CLICK = 4,
  BUTTON_EVENT_LONG_PRESS = 5,
};

#endif // _BUTTON_EVENT_H
