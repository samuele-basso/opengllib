#pragma once
#include "Event.h"

namespace Jam
{

	class ViewportResizeEvent : public Event
	{
	public:
		virtual EventType GetType() { return EventType::ViewportResizeEvent; }
	public:
		ViewportResizeEvent(float f_XSize, float f_YSize)
			: p_XSize(f_XSize), p_YSize(f_YSize) { }
		~ViewportResizeEvent() = default;
		ViewportResizeEvent(const ViewportResizeEvent&) = default;

		inline float GetXSize() { return p_XSize; }
		inline float GetYSize() { return p_YSize; }
	private:
		float p_XSize, p_YSize;
	};

}