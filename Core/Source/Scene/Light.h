#pragma once

#include "Transform.h"

namespace Jam
{

	struct Light
	{
		Light()
			: Color(1.0f)
		{
		}

		glm::vec3 Color;
	};

	struct PointLight : public Light, public Translation
	{
		PointLight()
		{
		}
	};

	struct SunLight : public Light, public Rotation
	{
		SunLight()
			: Direction({ 0.0f, -1.0f, 0.0f })
		{
		}

		void UpdateDirection(glm::vec3& f_Rotation);

		glm::vec3 Direction;
	};

	struct SpotLight : public Light, public Rotation, public Translation
	{
		SpotLight()
			: Direction({ 0.0f, -1.0f, 0.0f }), AlphaCosine(0.5), BetaCosine(0.4)
		{
		}

		void UpdateDirection(glm::vec3& f_Rotation);

		glm::vec3 Direction;

		float AlphaCosine;
		float BetaCosine;
	};

}