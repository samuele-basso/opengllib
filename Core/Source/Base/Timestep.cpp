#include "gfxpch.h"
#include "Timestep.h"

namespace Jam
{

	float Timestep::p_Step = 0;

	Timestep::Timestep()
		: p_TimeConstructor(glfwGetTime())
	{
	}

	Timestep::~Timestep()
	{
		float time = glfwGetTime();
		Timestep::Set(time - p_TimeConstructor);
		p_TimeConstructor = time;
	}

}