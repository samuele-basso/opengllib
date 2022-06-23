#pragma once
#include "Window.h"

#include <memory>

namespace Jam
{

	class Input
	{
	public:
		Input() = delete;
		~Input() = delete;
		Input(const Input&) = delete;

	public:
		static void Init(Window* f_Window);

		static bool IsKeyPressed(int f_KeyCode);
		static bool IsMouseKeyPressed(int f_KeyCode);

	private:
		static Window* p_Window;
	};

}