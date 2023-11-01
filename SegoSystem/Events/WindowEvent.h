#pragma once
#include "Event.h"


class WindowChangeSize : public Event
{
public:
WindowChangeSize(int w , int h) :W_width(w) , W_height(h) {}

protected:
EVENT_CLASS_TYPE(WindowClose)

EVENT_CLASS_CATEGORY(EventCategoryApplicayion | EventCategoryInput)

private:
int W_width;
int W_height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplicayion)
};