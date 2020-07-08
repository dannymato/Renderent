#pragma once

#include <sstream>

#include "Renderent/Event/Event.h"

namespace Renderent {

	class RENDERENT_API WindowClosedEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

		EVENT_CLASS_TYPE(WindowClosed)

		std::string ToString() const override {
			return std::string("WindowClosedEvent");
		}

	};

	class RENDERENT_API WindowResizedEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowResized)

		WindowResizedEvent(int width, int height)
			: m_Width(width), m_Height(height) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		int m_Width, m_Height;
	};

	class RENDERENT_API WindowMovedEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowMoved)

		WindowMovedEvent(int x, int y)
			: m_X(x), m_Y(y) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_X << ", " << m_Y;
			return ss.str();
		}

	private:
		int m_X, m_Y;

	};

	class RENDERENT_API WindowLostFocusEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowLostFocus)

		std::string ToString() const override{
			return "WindowLostFocusEvent";
		}
	};

	class RENDERENT_API WindowFocusedEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowFocused)

		std::string ToString() const override {
			return "WindowFocusedEvent";
		}
	};

}