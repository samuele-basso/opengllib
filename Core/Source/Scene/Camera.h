#pragma once
#include "Base/UUID.h"

#include "Transform.h"

namespace Jam
{

	enum class CameraProjection
	{
		Perspective,
		Orthographic
	};

	class Camera : public Rotation, public Translation
	{
	public:
		Camera();
		~Camera() = default;
		Camera(const Camera&) = default;

		void SetPosition(glm::vec3& f_Position);
		void SetRotation(glm::vec3& f_Rotation);

		void RecalculateViewMatrix();

		void UpdateType(CameraProjection f_Type);

		void SetProjection(glm::mat4& f_Projection);

		void UpdateProjection(float f_Aspect);
		void UpdateProjection();

		glm::mat4& GetViewProjection() { return p_ViewProjection; }
		glm::mat4& GetProjection() { return p_Projection; }

		glm::vec3& GetFront() { return p_Front; }
		glm::vec3& GetUp() { return p_Up; }
		glm::vec3& GetRight() { return p_Right; }

	public:
		CameraProjection ProjectionType;

		float Near;
		float Far;
		float FOV;
		float Size;

		float Aspect;

	private:
		glm::mat4 p_Projection;
		glm::mat4 p_View;
		glm::mat4 p_ViewProjection;

		glm::vec3 p_Front;
		glm::vec3 p_Up;
		glm::vec3 p_Right;

		glm::vec3 p_WorldUp;
	};

}