#pragma once
#include "Event.h"

class WindowEvent:public Event
{
protected:
EVENT_CLASS_TYPE(WindowTyped)

EVENT_CLASS_CATEGORY(EventCategoryApplicayion | EventCategoryInput)

protected:

} ;


class WindowClose : public WindowEvent
{
protected:
EVENT_CLASS_TYPE(WindowClose)

EVENT_CLASS_CATEGORY(EventCategoryApplicayion | EventCategoryInput)
public:
WindowClose() {}

private:

};

class WindowChangeSize : public WindowEvent
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