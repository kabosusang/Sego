#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <sstream>

#define BIT(x) (1<<x)


enum class EventType
{
    None = 0,
    WindowClose,WindowResize,WindowFocus,WindowsLostFocus,WindowMoved,WindowTyped,
    AppTick,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,MouseType
};
enum EventCategory //事件分类枚举
{
	None = 0,
	EventCategoryApplicayion	= BIT(0),
	EventCategoryInput			= BIT(1),
	EventCategoryKeyboard		= BIT(2),
	EventCategoryMouse			= BIT(3),
	EventCategoryMouseButton	= BIT(4)

};
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() {return EventType::type;}\
								virtual EventType GetEventType() const { return GetStaticType();}\
                                const char* GetName() const {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatch;
public:
    virtual EventType GetEventType() const = 0;
    
    
    virtual const char* GetName() const =0;
    virtual std::string ToString() const { return GetName(); }
	inline bool HandledStatus() { return m_Handled; }
	virtual int GetCategoryFlags() const = 0;

protected:
	bool m_Handled = false; //查看一个事件是否被处理
};


class EventDispatch
{
template<class T>
using EventFn = std::function<bool(T&)>;

template<typename T>
bool Dispatch(EventFn<T> func)
{
	if (m_Event.GetEventType() == T::GetStaticType())
	{
		m_Event.m_Handled = func(*(T*)&m_Event);
		return true;
	}
	return false;
}

private:
Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}














#endif