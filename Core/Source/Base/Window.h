#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include "Base/Global.h"

namespace Jam
{

	class Window
	{
	public:
		Window();

		void Init();

		~Window();
		Window(const Window&) = default;

		bool ShouldClose();
		void Update();

		void SetVSync(bool f_Enabled);

		int IsIconified();

		int GetWidth() { return p_State.Width; }
		int GetHeight() { return p_State.Height; }

		GLFWwindow* GetGLFWWindow() { return p_Window; }

		void CaptureCursor(bool f_Enabled);

	private:
		void InitCallbacks();

		GLFWwindow* p_Window;
		Global::WindowState& p_State;
	};

}