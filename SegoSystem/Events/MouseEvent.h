#pragma once

#include "Event.h"


class MouseEvent :public Event
{
public:
EVENT_CLASS_TYPE(MouseType)
EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

};

class MouseButtonPressedEvent : public MouseEvent
{
public:
EVENT_CLASS_TYPE(MouseButtonPressed)
EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

inline int GetMouseCode(){return MousePressCode;}
public:
MouseButtonPressedEvent(int MouseCode) : MousePressCode(MouseCode) {}

std::string ToString() const override
{
std::stringstream ss;
ss << "MouseButtonPressedEvent: " << MousePressCode << std::endl;
return ss.str();
}
private:
int MousePressCode;

};

class MouseButtonReleasedEvent : public MouseEvent
{
public:
EVENT_CLASS_TYPE(MouseButtonReleased)
EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
public:
MouseButtonReleasedEvent(int MouseCode) : MousePressCode(MouseCode) {}
inline int GetMouseCode(){return MousePressCode;}
std::string ToString() const override
{
std::stringstream ss;
ss << "MouseButtonReleasedEvent: " << MousePressCode << std::endl;
return ss.str();
}

int MousePressCode;

};


class MouseMoveEvent : public MouseEvent
{
public:
MouseMoveEvent(float mousex ,float mousey) :x(mousex) , y(mousey) {}
EVENT_CLASS_TYPE(MouseMoved)
EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

inline float GetMouseX() { return x;}
inline float GetMouseY() { return y;}

protected:  

std::string ToString() const override
{
std::stringstream ss;
ss << "MouseButtonPressedEvent: " << x  << " , "<< y << std::endl;
return ss.str();
}
private:
float x;
float y;

};

class MouseScrollEvent : public MouseEvent
{
public:
MouseScrollEvent(float x,float y):xffset(x),yffset(y) {}
inline float GetScrollX() { return xffset;}
inline float GetScrollY() { return yffset;}

public:  
EVENT_CLASS_TYPE(MouseScrolled)
EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)


std::string ToString() const override
{
std::stringstream ss;
ss << "MouseScrollEvent: " <<xffset << ", " << yffset << std::endl;
return ss.str();
}

private:
float xffset;
float yffset;

};








