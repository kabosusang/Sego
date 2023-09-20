#pragma once

#include <Event.h>

class KeyEvent :public Event
{

protected:
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

KeyRealseEvent(int KeyCode) : KeyEvent(KeyCode) {}

private:

};






