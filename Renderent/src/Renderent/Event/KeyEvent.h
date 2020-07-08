#pragma once
#include <sstream>

#include "Renderent/Event/Event.h"

namespace Renderent {

	class RENDERENT_API KeyEvent : public Event {
	
	public:
		int GetKeyCode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}
		int m_KeyCode;
		
	};

	class RENDERENT_API KeyPressedEvent : public KeyEvent {

	public:
		KeyPressedEvent(int keycode, bool isRepeated) :
			KeyEvent(keycode), m_isRepeated(isRepeated) {}

		inline bool GetIsRepeated() const { return m_isRepeated; }

		EVENT_CLASS_TYPE(KeyPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " Is Repeated: " << m_isRepeated;
			return ss.str();
		}

	private:
		bool m_isRepeated;
	};

	class RENDERENT_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) :
			KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

	class RENDERENT_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode) :
			KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

	};



}