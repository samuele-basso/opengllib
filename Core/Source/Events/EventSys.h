#pragma once

#include <vector>

#include "AppEvent.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "ViewportEvent.h"
#include "WindowEvent.h"

namespace Jam
{

	template <typename T>
	using Function = std::function<void(T)>*;

	class EventSys
	{
	public:
		EventSys() = delete;
		~EventSys() = delete;
		EventSys(const EventSys&) = delete;
	public:
		template <typename T>
		static std::vector<Function<T>>& GetEventCallbackInstance()
		{
			static std::vector<Function<T>> instance;
			return instance;
		}
	};

	template <typename T>
	class Publisher
	{
	public:
		Publisher() = delete;
		~Publisher() = delete;
		Publisher(const Publisher&) = delete;
	public:
		static void Subscribe(Function<T> f_S)
		{
			EventSys::GetEventCallbackInstance<T>().push_back(f_S);
		}

		static void Propagate(T a)
		{
			for (Function<T> s : EventSys::GetEventCallbackInstance<T>())
			{
				(*s)(a);
			}
		}
	};

	class EventQueue
	{
	public:
		EventQueue() = delete;
		~EventQueue() = delete;
		EventQueue(const EventQueue&) = delete;
	public:
		static void Push(Event* f_E)
		{
			GetEventQueue().push_back(f_E);
		}

		static void Process()
		{
			static std::vector<Event*>& q = GetEventQueue();
			Event* e = nullptr;
			while (!q.empty())
			{
				e = q.front();

				switch (e->GetType())
				{
				case EventType::None:
					Publisher<Event*>::Propagate((Event*)e);
					break;
				case EventType::WindowCloseEvent:
					Publisher<WindowCloseEvent*>::Propagate((WindowCloseEvent*)e);
					break;
				case EventType::WindowFocusEvent:
					Publisher<WindowFocusEvent*>::Propagate((WindowFocusEvent*)e);
					break;
				case EventType::WindowLostFocusEvent:
					Publisher<WindowLostFocusEvent*>::Propagate((WindowLostFocusEvent*)e);
					break;
				case EventType::WindowResizeEvent:
					Publisher<WindowResizeEvent*>::Propagate((WindowResizeEvent*)e);
					break;
				case EventType::WindowMoveEvent:
					Publisher<WindowMoveEvent*>::Propagate((WindowMoveEvent*)e);
					break;
				case EventType::WindowMinimizeEvent:
					Publisher<WindowMinimizeEvent*>::Propagate((WindowMinimizeEvent*)e);
					break;
				case EventType::WindowUnminimizeEvent:
					Publisher<WindowUnminimizeEvent*>::Propagate((WindowUnminimizeEvent*)e);
					break;
				case EventType::ViewportResizeEvent:
					Publisher<ViewportResizeEvent*>::Propagate((ViewportResizeEvent*)e);
					break;
				case EventType::MouseMoveEvent:
					Publisher<MouseMoveEvent*>::Propagate((MouseMoveEvent*)e);
					break;
				case EventType::MouseScrollEvent:
					Publisher<MouseScrollEvent*>::Propagate((MouseScrollEvent*)e);
					break;
				case EventType::MouseButtonPressEvent:
					Publisher<MouseButtonPressEvent*>::Propagate((MouseButtonPressEvent*)e);
					break;
				case EventType::MouseButtonReleaseEvent:
					Publisher<MouseButtonReleaseEvent*>::Propagate((MouseButtonReleaseEvent*)e);
					break;
				case EventType::KeyPressEvent:
					Publisher<KeyPressEvent*>::Propagate((KeyPressEvent*)e);
					break;
				case EventType::KeyReleaseEvent:
					Publisher<KeyReleaseEvent*>::Propagate((KeyReleaseEvent*)e);
					break;
				case EventType::KeyTypeEvent:
					Publisher<KeyTypeEvent*>::Propagate((KeyTypeEvent*)e);
					break;
				case EventType::AppTickEvent:
					Publisher<AppTickEvent*>::Propagate((AppTickEvent*)e);
					break;
				}

				q.erase(q.begin());
				delete e;
			}
		}

	private:
		static std::vector<Event*>& GetEventQueue()
		{
			static std::vector<Event*> instance;
			return instance;
		}
	};

}