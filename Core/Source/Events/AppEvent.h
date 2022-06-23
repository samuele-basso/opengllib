#pragma once

#include "Event.h"

namespace Jam
{

	class AppTickEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::AppTickEvent; }
	public:
		AppTickEvent() = default;
		~AppTickEvent() = default;
		AppTickEvent(const AppTickEvent&) = default;
	};

}