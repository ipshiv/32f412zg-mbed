/**
 * @file button_handler.h
 * @author Ivan Shivarev (ivan.shivarev@huld.io)
 * @brief
 * @version 0.1
 * @date 2022-07-05
 *
 * @copyright Copyright (c) 2022 Huld Oy
 *
 */

#ifndef _BUTTON_HANDLER_H
#define _BUTTON_HANDLER_H

#include "button_event.h"
#include "mbed.h"
#include <stdint.h>

class ButtonHandler {
public:
  ButtonHandler();
  int attachButton(uint32_t button, void (*func)(int));
  int registerEvent(uint32_t button, enum button_event evt_type,
                    uint16_t evt_time);
};

#endif // _BUTTON_HANDLER_H
