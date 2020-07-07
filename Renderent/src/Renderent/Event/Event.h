#pragma once

#include <functional>
#include <string>

#include "Renderent/Core/Core.h"

namespace Renderent {

	enum class EventType {
		None = 0,
		WindowClosed, WindowResized, WindowMoved, WindowLostFocus, WindowFocused,
		KeyPressed, KeyReleased, KeyTyped,
		MousePressed, MouseReleased, MouseScrolled, MouseMoved
	};

	enum EventCategory {
		None = 0,
		EventCategoryWindow = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryApp = BIT(4),
		EventCategoryInput = BIT(5)
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategory() const override {return category;}

	class RENDERENT_API Event {
		friend class EventDispatcher;
	public:

		

		virtual EventType GetEventType() const = 0;
		virtual int GetEventCategory() const = 0;
		virtual std::string ToString() const { return GetName(); }
		virtual const char* GetName() const = 0;

		inline bool IsInCategory(EventCategory category) {
			return GetEventCategory() & category;
		}
		bool Handled = false;

	};

	class EventDispatcher {
		template<typename T>

		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}