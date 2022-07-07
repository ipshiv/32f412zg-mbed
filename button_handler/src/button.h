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

#include "mbed.h"
#include "button_events.h"
#include "button_event.h"

using namespace std::chrono;

// TODO Refactor to compilator variable
#define MAX_EVENTS_PER_BUTTON 4

class Button {
    public:
	Button(uint32_t id, EventQueue *queue, Timer *t, Callback<void(int)> cb)
	{
		self_id = id;
		queue = queue;
		t = t;
		cb = cb;
	}

	uint32_t id(void)
	{
		return self_id;
	}

	void buttonPressed(void)
	{
		if (events.empty())
			return;
		uint64_t timestamp =
			duration_cast<milliseconds>(t->elapsed_time()).count();
		for (auto &event : events) {
			bool is_emmited =
				event->updateButtonState(true, timestamp);
			if (is_emmited)
				queue->call(cb, event->type());
		}
	}

	void buttonReleased(void)
	{
		if (events.empty())
			return;
		uint64_t timestamp =
			duration_cast<milliseconds>(t->elapsed_time()).count();
		for (auto &event : events) {
			bool is_emmited =
				event->updateButtonState(false, timestamp);
			if (is_emmited)
				queue->call(cb, event->type());
		}
	}

	bool registerEvent(
		enum button_event evt_type,
		uint16_t event_length_ms,
		uint16_t debounce)
	{
		auto last_element = events.size();
		if (last_element == MAX_EVENTS_PER_BUTTON)
			return false;
		if (!last_element)
			last_element = 1;

		ButtonEvent *new_event;
		switch (evt_type) {
		case (BUTTON_EVENT_PUSH):
			new_event = new PushEvent(
				evt_type, event_length_ms, debounce);
			break;
		case (BUTTON_EVENT_RELEASED):
			new_event = new ReleaseEvent(
				evt_type, event_length_ms, debounce);
			break;
		case (BUTTON_EVENT_CLICK):
			new_event = new ClickEvent(
				evt_type, event_length_ms, debounce);
			break;
		case (BUTTON_EVENT_DOUBLE_CLICK):
			new_event = new DoubleClickEvent(
				evt_type, event_length_ms, debounce);
			break;
		case (BUTTON_EVENT_LONG_PRESS):
			new_event = new LongPress(
				evt_type, event_length_ms, debounce);
			break;
		default:
			return false;
		}

		if (new_event == nullptr)
			return false;

		events[last_element - 1] = new_element;
	}

    private:
	std::array<ButtonEvent *, MAX_EVENTS_PER_BUTTON> events;
	EventQueue *queue;
	Callback<void(int)> cb;
	Timer *t;
	uint32_t self_id;
};

#endif // _BUTTON_H
