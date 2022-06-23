#pragma once
#include "Event.h"

namespace Jam
{

	class WindowCloseEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowCloseEvent; }
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;
		WindowCloseEvent(const WindowCloseEvent&) = default;
	};

	class WindowFocusEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowFocusEvent; }
	public:
		WindowFocusEvent() = default;
		~WindowFocusEvent() = default;
		WindowFocusEvent(const WindowFocusEvent&) = default;
	};

	class WindowLostFocusEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowLostFocusEvent; }
	public:
		WindowLostFocusEvent() = default;
		~WindowLostFocusEvent() = default;
		WindowLostFocusEvent(const WindowLostFocusEvent&) = default;
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowMinimizeEvent; }
	public:
		WindowMinimizeEvent() = default;
		~WindowMinimizeEvent() = default;
		WindowMinimizeEvent(const WindowMinimizeEvent&) = default;
	};

	class WindowUnminimizeEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowUnminimizeEvent; }
	public:
		WindowUnminimizeEvent() = default;
		~WindowUnminimizeEvent() = default;
		WindowUnminimizeEvent(const WindowUnminimizeEvent&) = default;
	};

	class WindowResizeEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowResizeEvent; }
	public:
		WindowResizeEvent(float f_XSize, float f_YSize)
			: p_XSize(f_XSize), p_YSize(f_YSize) { }
		~WindowResizeEvent() = default;
		WindowResizeEvent(const WindowResizeEvent&) = default;

		inline float GetXSize() { return p_XSize; }
		inline float GetYSize() { return p_YSize; }
	private:
		float p_XSize, p_YSize;
	};

	class WindowMoveEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::WindowMoveEvent; }
	public:
		WindowMoveEvent(float f_XPos, float f_YPos)
			: p_XPos(f_XPos), p_YPos(f_YPos) { }
		~WindowMoveEvent() = default;
		WindowMoveEvent(const WindowMoveEvent&) = default;

		inline float GetXPos() { return p_XPos; }
		inline float GetYPos() { return p_YPos; }
	private:
		float p_XPos, p_YPos;
	};

}