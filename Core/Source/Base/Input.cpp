#include "gfxpch.h"

#include "Input.h"

#include "Events/EventSys.h"
#include "Events/AppEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Jam
{

	Window* Input::p_Window = nullptr;

	bool Input::IsKeyPressed(int f_KeyCode)
	{
		int state = glfwGetKey(p_Window->GetGLFWWindow(), f_KeyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseKeyPressed(int f_KeyCode)
	{
		int state = glfwGetMouseButton(p_Window->GetGLFWWindow(), f_KeyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	void Input::Init(Window* f_Window)
	{
		p_Window = f_Window;

		glfwSetKeyCallback(p_Window->GetGLFWWindow(), [](GLFWwindow* f_Window, int f_Key, int f_Scancode, int f_Action, int f_Mods)
			{
				switch (f_Action)
				{
				case GLFW_PRESS:
				{
					KeyPressEvent* e = new KeyPressEvent(f_Key, 0);
					EventQueue::Push(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressEvent* e = new KeyPressEvent(f_Key, 1);
					EventQueue::Push(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleaseEvent* e = new KeyReleaseEvent(f_Key);
					EventQueue::Push(e);
					break;
				}
				}
			});

		glfwSetCharCallback(p_Window->GetGLFWWindow(), [](GLFWwindow* f_Window, uint32_t f_Keycode)
			{
				KeyTypeEvent* e = new KeyTypeEvent(f_Keycode);
				EventQueue::Push(e);
			});

		glfwSetMouseButtonCallback(p_Window->GetGLFWWindow(), [](GLFWwindow* f_Window, int f_Button, int f_Action, int f_Mods)
			{
				switch (f_Action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressEvent* e = new MouseButtonPressEvent(f_Button);
					EventQueue::Push(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleaseEvent* e = new MouseButtonReleaseEvent(f_Button);
					EventQueue::Push(e);
					break;
				}
				}
			});

		glfwSetScrollCallback(p_Window->GetGLFWWindow(), [](GLFWwindow* f_Window, double f_XOffset, double f_YOffset)
			{
				MouseScrollEvent* e = new MouseScrollEvent(f_XOffset, f_YOffset);
				EventQueue::Push(e);
			});

		glfwSetCursorPosCallback(p_Window->GetGLFWWindow(), [](GLFWwindow* f_Window, double f_XPos, double f_YPos)
			{
				MouseMoveEvent* e = new MouseMoveEvent(f_XPos, f_YPos);
				EventQueue::Push(e);
			});
	}

}