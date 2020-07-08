#pragma once

#include <sstream>

#include "Renderent/Event/Event.h"

namespace Renderent {


	class RENDERENT_API MouseButtonEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		inline int GetMouseButton() { return m_Button; }

	protected:
		int m_Button;
		MouseButtonEvent(int button) : m_Button(button) {}
	};

	class RENDERENT_API MousePressedEvent : public MouseButtonEvent {
	public:
		MousePressedEvent(int button) : MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MousePressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MousePressedEvent: " << m_Button;
			return ss.str();
		}
	};

	class RENDERENT_API MouseReleasedEvent : public MouseButtonEvent {
	public:
		EVENT_CLASS_TYPE(MouseReleased)
		

		MouseReleasedEvent(int button) : MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << m_Button;
			return ss.str();
		}
	};

	class RENDERENT_API MouseMovedEvent : public Event {
	public:
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

			MouseMovedEvent(float x, float y) : m_X(x), m_Y(y) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_X << ", " << m_Y;
			return ss.str();
		}

		float GetX() { return m_X; }
		float GetY() { return m_Y; }

	private:
		float m_X, m_Y;
	};

	class RENDERENT_API MouseScrolledEvent : public Event {
	public:
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

		MouseScrolledEvent(float xOffset, float yOffset) 
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		float GetXOffset() { return m_xOffset; }
		float GetYOffset() { return m_yOffset; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
			return ss.str();
		}


	private:
		float m_xOffset, m_yOffset;
	};
}