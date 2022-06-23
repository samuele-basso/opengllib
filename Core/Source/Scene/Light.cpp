#include "gfxpch.h"
#include "Light.h"

namespace Jam
{

	void SunLight::UpdateDirection(glm::vec3& f_Rotation)
	{
		SetRotation(f_Rotation);
		Direction = RotationMatrix * glm::vec4(0.0f, -1.0f, 0.0f, 0.0);
	}

	void SpotLight::UpdateDirection(glm::vec3& f_Rotation)
	{
		SetRotation(f_Rotation);
		Direction = RotationMatrix * glm::vec4(0.0f, -1.0f, 0.0f , 0.0);
	}

}