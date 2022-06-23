#pragma once

#include "Scene/Camera.h"

#include "Events/AppEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ViewportEvent.h"

namespace Sandbox
{

	class EditorCameraController
	{
	public:
		EditorCameraController();
		~EditorCameraController() = default;
		EditorCameraController(const EditorCameraController&) = default;

		Jam::Camera& GetCamera() { return p_Camera; }

	private:
		void OnViewportResize(Jam::ViewportResizeEvent* f_E);
		void OnMouseMove(Jam::MouseMoveEvent* f_E);
		void OnMouseScroll(Jam::MouseScrollEvent* f_E);

		Jam::Camera p_Camera;

		float p_TranslationSpeed;
		float p_RotationSpeed;
	};


}