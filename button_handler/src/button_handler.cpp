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

#include "mbed.h"
#include "button.h"
#include "button_handler.h"

#define MAX_BUTTONS 4

Timer t;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread button_thread;

std::array<Button *, MAX_BUTTONS 4> buttons;

ButtonHandler::ButtonHandler()
{
	t.start();
	button_thread.start(callback(&queue, &EventQueue::dispatch_forever));
}

ButtonHandler::attachButton(uint32_t button, Callback<void(int)> func)
{
	auto last = buttons.size();
	if (last == MAX_BUTTONS)
		return -1;

	if (!last)
		last = 1;

	for (auto &b : buttons) {
		if (b.id() == button)
			return -1;
	}

	Button *new_button = new Button(button, &queue, &t, func);
	InterruptIn *new_isr = new InterruptIn(button);
	new_isr->rise(&new_button->buttonPressed);
	new_isr->fall(&new_button->buttonReleased);
	buttons[last - 1] = new_button;

	return 0;
}

ButtonHandler::registerEvent(
	uint32_t button,
	enum button_event evt_type,
	uitn16_t evt_time)
{
	if (buttons.empty())
		return -1;

	for (auto &b : buttons) {
		if (b.id() == button) {
			if (b.registerEvent(evt_type, evt_time, 200))
				return 0;
			break;
		}
	}

	return -1
}
