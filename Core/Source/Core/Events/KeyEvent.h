#pragma once

#include "Events/Event.h"

namespace Jah {

	class KeyEvent : public Event
	{
	public:
		uint16_t GetKeyCode() const { return m_KeyCode; }
		int GetCategoryFlags() const { return EventCategoryKeyboard | EventCategoryInput; }

	protected:
		KeyEvent(const uint16_t keycode)
			: m_KeyCode(keycode)
		{
		}

		uint16_t m_KeyCode = 0;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:

		KeyPressedEvent(const uint16_t keycode, uint32_t repeatCount = 0)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		bool GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	private:
		uint32_t m_RepeatCount = 0;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(uint16_t keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			return "KeyReleasedEvent: " + std::to_string(m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyPressed)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	};


}