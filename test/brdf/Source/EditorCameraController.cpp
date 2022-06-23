#include "EditorCameraController.h"

#include "Base/Timestep.h"

#include "Base/Input.h"

#include "Events/EventSys.h"

namespace Sandbox
{

	EditorCameraController::EditorCameraController()
		: p_TranslationSpeed(1.0f), p_RotationSpeed(0.1f)
	{
		std::function<void(Jam::ViewportResizeEvent*)>* viewportResizeEvent = new std::function<void(Jam::ViewportResizeEvent*)>();
		*viewportResizeEvent = std::bind(&EditorCameraController::OnViewportResize, this, std::placeholders::_1);
		Jam::Publisher<Jam::ViewportResizeEvent*>::Subscribe(viewportResizeEvent);

		std::function<void(Jam::MouseMoveEvent*)>* mouseMoveEvent = new std::function<void(Jam::MouseMoveEvent*)>();
		*mouseMoveEvent = std::bind(&EditorCameraController::OnMouseMove, this, std::placeholders::_1);
		Jam::Publisher<Jam::MouseMoveEvent*>::Subscribe(mouseMoveEvent);

		std::function<void(Jam::MouseScrollEvent*)>* mouseScrollEvent = new std::function<void(Jam::MouseScrollEvent*)>();
		*mouseScrollEvent = std::bind(&EditorCameraController::OnMouseScroll, this, std::placeholders::_1);
		Jam::Publisher<Jam::MouseScrollEvent*>::Subscribe(mouseScrollEvent);
	}

	void EditorCameraController::OnViewportResize(Jam::ViewportResizeEvent* f_E)
	{
		p_Camera.UpdateProjection(f_E->GetXSize() / f_E->GetYSize());
	}

	void EditorCameraController::OnMouseMove(Jam::MouseMoveEvent* f_E)
	{
		float ts = Jam::Timestep::Get();

		float newX = f_E->GetX();
		float newY = f_E->GetY();

		static float oldX = 0;
		static float oldY = 0;

		float deltaX = oldX - newX;
		float deltaY = oldY - newY;

		if (Jam::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			if (Jam::Input::IsMouseKeyPressed(GLFW_MOUSE_BUTTON_MIDDLE))
			{
				p_Camera.SetPosition(
					p_Camera.TranslationVector +
					(p_Camera.GetRight() * deltaX * p_TranslationSpeed * ts) +
					(-p_Camera.GetUp() * deltaY * p_TranslationSpeed * ts)
				);
			}

			if (Jam::Input::IsMouseKeyPressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				p_Camera.Size += deltaY * ts;
				p_Camera.FOV += deltaY * ts;

				p_Camera.UpdateProjection();
			}
		}

		oldY = newY;
		oldX = newX;
	}

	void EditorCameraController::OnMouseScroll(Jam::MouseScrollEvent* f_E)
	{
		glm::vec3 position = p_Camera.TranslationVector + (p_Camera.GetFront() * f_E->GetYOffset() * p_TranslationSpeed);
		p_Camera.SetPosition(position);
	}

}