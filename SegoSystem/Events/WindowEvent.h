#pragma once
#include "Event.h"


class WindowChangeSize : public Event
{
public:
WindowChangeSize(int w , int h) :W_width(w) , W_height(h) {}

std::string ToString() const override
{ 
    std::stringstream ss;
    ss << "WindowChangeSize: " << W_width <<", " << W_height << std::endl;
    return ss.str();
}
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
std::string ToString() const override
{ 
    std::stringstream ss;
    ss << "WindowClose_Event "  << std::endl;
    return ss.str();
}
	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplicayion)
};