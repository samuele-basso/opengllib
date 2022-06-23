#pragma once

#include <glm/glm.hpp>

namespace Jam
{

	class Rotation
	{
	public:
		Rotation();
		virtual ~Rotation() = default;
		Rotation(const Rotation&) = default;

		void SetRotation(glm::vec3& f_Rotation);

		glm::mat4 RotationMatrix;
		glm::vec3 RotationVector;
	};

	class Scale
	{
	public:
		Scale();
		virtual ~Scale() = default;
		Scale(const Scale&) = default;

		void SetScale(glm::vec3& f_Scale);

		glm::mat4 ScaleMatrix;
		glm::vec3 ScaleVector;
	};

	class Translation
	{
	public:
		Translation();
		virtual ~Translation() = default;
		Translation(const Translation&) = default;

		void SetPosition(glm::vec3& f_Position);

		glm::mat4 TranslationMatrix;
		glm::vec3 TranslationVector;
	};

	class Transform : public Rotation, public Scale, public Translation
	{
	public:
		Transform();
		virtual ~Transform() = default;
		Transform(const Transform&) = default;

		void RecalculateModelMatrix();

		glm::mat4 ModelMatrix;
	};

}