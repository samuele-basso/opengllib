#include "gfxpch.h"
#include "Transform.h"

namespace Jam
{

	Rotation::Rotation()
		: RotationVector(0.0f), RotationMatrix(1.0f)
	{
	}

	void Rotation::SetRotation(glm::vec3& f_Rotation)
	{
		RotationVector = f_Rotation;

		RotationMatrix = glm::toMat4(
			glm::angleAxis(glm::radians(RotationVector.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::angleAxis(glm::radians(RotationVector.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::angleAxis(glm::radians(RotationVector.z), glm::vec3(0.0f, 0.0f, 1.0f))
		);
	}

	Scale::Scale()
		: ScaleVector(1.0f), ScaleMatrix(1.0f)
	{
	}

	void Scale::SetScale(glm::vec3& f_Scale)
	{
		ScaleVector = f_Scale;

		ScaleMatrix = glm::scale(glm::mat4(1.0f), ScaleVector);
	}

	Translation::Translation()
		: TranslationVector(0.0f), TranslationMatrix(1.0f)
	{
	}

	void Translation::SetPosition(glm::vec3& f_Position)
	{
		TranslationVector = f_Position;

		TranslationMatrix = glm::translate(glm::mat4(1.0f), TranslationVector);
	}

	Transform::Transform()
		: ModelMatrix(1.0f)
	{
	}

	void Transform::RecalculateModelMatrix()
	{
		ModelMatrix = TranslationMatrix * ScaleMatrix * RotationMatrix;
	}

}