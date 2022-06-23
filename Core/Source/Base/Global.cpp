#include "gfxpch.h"
#include "Global.h"

#include "Events/ViewportEvent.h"
#include "Events/EventSys.h"

namespace Jam
{

	Global::ViewportState Global::GlobalViewportState;

	Global::WindowState Global::GlobalWindowState;

	Global::OpenGLVersion Global::GlobaOpenGLVersion;

	void Global::Init()
	{
		std::function<void(ViewportResizeEvent*)>* viewportResizeEvent = new std::function<void(ViewportResizeEvent*)>([&](ViewportResizeEvent* f_E) {
			GlobalViewportState.Width = f_E->GetXSize();
			GlobalViewportState.Height = f_E->GetYSize();
			GlobalViewportState.Valid = true;
		});
		Publisher<ViewportResizeEvent*>::Subscribe(viewportResizeEvent);

		std::function<void(WindowResizeEvent*)>* windowResizeEvent = new std::function<void(WindowResizeEvent*)>([&](WindowResizeEvent* f_E) {
			GlobalWindowState.Width = f_E->GetXSize();
			GlobalWindowState.Height = f_E->GetYSize();
		});
		Publisher<WindowResizeEvent*>::Subscribe(windowResizeEvent);
	}

}