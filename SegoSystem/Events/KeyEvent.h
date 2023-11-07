#pragma once

#include "Events/Event.h"

class KeyEvent :public Event
{
public:
inline int GetKeyCode() const { return Key_Code; }
public:
KeyEvent(int KeyCode) : Key_Code(KeyCode) {}
EVENT_CLASS_TYPE(KeyTyped)

EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

int Key_Code;

};

class KeyDownEvent : public KeyEvent
{
public:
std::string ToString() const override
{ 
    std::stringstream ss;
    ss << "KeyDownEvent: " << Key_Code << std::endl;
    return ss.str();
}
KeyDownEvent(int KeyCode , int keyCount) : KeyEvent(KeyCode),m_Count(keyCount) {}
EVENT_CLASS_TYPE(KeyPressed)

private:
int m_Count;
};

class KeyDownRepeate : public KeyEvent
{
public:
std::string ToString() const override
{ 
    std::stringstream ss;
    ss << "KeyDownRepeate: " << Key_Code << std::endl;
    return ss.str();
}
KeyDownRepeate(int KeyCode , int keyCount) : KeyEvent(KeyCode),m_Count(keyCount) {}
EVENT_CLASS_TYPE(KeyDownRepeate)

private:
int m_Count;
};



class KeyRealseEvent : public KeyEvent
{
public:
std::string ToString() const override
{ 
    std::stringstream ss;
    ss << "KeyRealseEvent: " << Key_Code << std::endl;
    return ss.str();
}
EVENT_CLASS_TYPE(KeyReleased)
KeyRealseEvent(int KeyCode) : KeyEvent(KeyCode) {}

private:

};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keycode)
		:KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << Key_Code ;
		return ss.str();
	}
	EVENT_CLASS_TYPE(KeyTyped)

};






