#pragma once
#include "Event.h"

namespace Jam
{

	class MouseMoveEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::MouseMoveEvent; }
	public:
		MouseMoveEvent(float f_X, float f_Y)
			: p_X(f_X), p_Y(f_Y) { }
		~MouseMoveEvent() = default;
		MouseMoveEvent(const MouseMoveEvent&) = default;

		inline float GetX() { return p_X; }
		inline float GetY() { return p_Y; }
	private:
		float p_X, p_Y;
	};

	class MouseScrollEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::MouseScrollEvent; }
	public:
		MouseScrollEvent(float f_XOffset, float f_YOffset)
			: p_XOffset(f_XOffset), p_YOffset(f_YOffset) { }
		~MouseScrollEvent() = default;
		MouseScrollEvent(const MouseScrollEvent&) = default;

		inline float GetXOffset() { return p_XOffset; }
		inline float GetYOffset() { return p_YOffset; }
	private:
		float p_XOffset, p_YOffset;
	};

	class MouseButtonPressEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::MouseButtonPressEvent; }
	public:
		MouseButtonPressEvent(int f_KeyCode)
			: p_KeyCode(f_KeyCode) { }
		~MouseButtonPressEvent() = default;
		MouseButtonPressEvent(const MouseButtonPressEvent&) = default;

		inline int GetKeyCode() { return p_KeyCode; }
	private:
		int p_KeyCode;
	};

	class MouseButtonReleaseEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::MouseButtonReleaseEvent; }
	public:
		MouseButtonReleaseEvent(int f_KeyCode)
			: p_KeyCode(f_KeyCode) { }
		~MouseButtonReleaseEvent() = default;
		MouseButtonReleaseEvent(const MouseButtonReleaseEvent&) = default;

		inline int GetKeyCode() { return p_KeyCode; }
	private:
		int p_KeyCode;
	};

}
