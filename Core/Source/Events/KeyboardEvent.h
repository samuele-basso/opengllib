#pragma once
#include "Event.h"

namespace Jam
{

	class KeyPressEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::KeyPressEvent; }
	public:
		KeyPressEvent(int f_KeyCode, int f_Repeat)
			: p_KeyCode(f_KeyCode), p_Repeat(f_Repeat) { }
		~KeyPressEvent() = default;
		KeyPressEvent(const KeyPressEvent&) = default;

		inline int GetKeyCode() { return p_KeyCode; }
		inline int GetRepeat() { return p_Repeat; }
	private:
		int p_KeyCode;
		int p_Repeat;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::KeyReleaseEvent; }
	public:
		KeyReleaseEvent(int f_KeyCode)
			: p_KeyCode(f_KeyCode) { }
		~KeyReleaseEvent() = default;
		KeyReleaseEvent(const KeyReleaseEvent&) = default;

		inline int GetKeyCode() { return p_KeyCode; }
	private:
		int p_KeyCode;
	};

	class KeyTypeEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::KeyTypeEvent; }
	public:
		KeyTypeEvent(int f_KeyCode)
			: p_KeyCode(f_KeyCode) { }
		~KeyTypeEvent() = default;
		KeyTypeEvent(const KeyTypeEvent&) = default;

		inline int GetKeyCode() { return p_KeyCode; }
	private:
		int p_KeyCode;
	};

}