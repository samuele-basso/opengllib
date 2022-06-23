#pragma once

namespace Jam
{

	class Timestep
	{
	public:
		Timestep();
		~Timestep();
		Timestep(const Timestep&) = delete;

	private:
		float p_TimeConstructor;

	public:
		static void Set(float f_Step) { p_Step = f_Step; }
		static float Get() { return p_Step; }

	private:
		static float p_Step;
	};

}