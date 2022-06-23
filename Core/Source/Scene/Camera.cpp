#include "gfxpch.h"
#include "Camera.h"

#include "Base/Log.h"

#include "Events/EventSys.h"
#include "Events/WindowEvent.h"

namespace Jam
{

	Camera::Camera()
		: FOV(45.0f), Size(1.0f), p_WorldUp(0.0f, 1.0f, 0.0f), Near(0.1f), Far(1000.0f),
		ProjectionType(CameraProjection::Perspective),
		p_Projection(1.0f), p_View(1.0f), p_ViewProjection(1.0f), Aspect(1.0)
	{
		RecalculateViewMatrix();
	}

	void Camera::SetPosition(glm::vec3& f_Position)
	{
		TranslationVector = f_Position;

		TranslationMatrix = glm::translate(glm::mat4(1.0f), TranslationVector);

		RecalculateViewMatrix();
	}

	void Camera::SetRotation(glm::vec3& f_Rotation)
	{
		RotationVector = f_Rotation;

		RotationMatrix = glm::toMat4(
			glm::angleAxis(glm::radians(RotationVector.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::angleAxis(glm::radians(RotationVector.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::angleAxis(glm::radians(RotationVector.z), glm::vec3(0.0f, 0.0f, 1.0f))
		);

		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		p_Front = glm::normalize(RotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

		p_Right = glm::normalize(glm::cross(p_Front, p_WorldUp));
		p_Up = glm::normalize(glm::cross(p_Right, p_Front));

		p_View = glm::lookAt(TranslationVector, TranslationVector + p_Front, p_Up);
		p_ViewProjection = p_Projection * p_View;
	}

	void Camera::UpdateType(CameraProjection f_Type)
	{
		ProjectionType = f_Type;
	}

	void Camera::SetProjection(glm::mat4& f_Projection)
	{
		p_Projection = f_Projection;
		p_ViewProjection = p_Projection * p_View;
	}

	void Camera::UpdateProjection(float f_Aspect)
	{
		Aspect = f_Aspect;
		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		switch (ProjectionType)
		{
		case CameraProjection::Orthographic:
			glm::mat4 ortho = glm::ortho(
				-Aspect / 2 * Size, Aspect / 2 * Size,
				-0.5f * Size, 0.5f * Size,
				Near, Far);
			SetProjection(ortho);
			break;
		case CameraProjection::Perspective:
			glm::mat4 perspective = glm::perspective(
				glm::radians(FOV), Aspect,
				Near, Far);
			SetProjection(perspective);
			break;
		default:
			JAM_ASSERT(false, "unknown projection type");
		}
	}

}