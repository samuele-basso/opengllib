#pragma once

#include <vector>

#include "Base/Log.h"

namespace Jam
{

	enum class EventType
	{
		None,
		WindowCloseEvent, WindowFocusEvent, WindowLostFocusEvent, WindowResizeEvent, WindowMoveEvent, WindowMinimizeEvent, WindowUnminimizeEvent,
		ViewportResizeEvent,
		MouseMoveEvent, MouseScrollEvent, MouseButtonPressEvent, MouseButtonReleaseEvent,
		KeyPressEvent, KeyReleaseEvent, KeyTypeEvent,
		AppTickEvent
	};

	class Event
	{
	public:
		virtual EventType GetType() { return EventType::None; }
	public:
		Event() = default;
		~Event() = default;
		Event(const Event&) = default;
	};

}