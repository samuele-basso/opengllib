#include "gfxpch.h"
#include "Window.h"

#include "Log.h"

#include "Events/EventSys.h"
#include "Events/WindowEvent.h"

#include "Global.h"

namespace Jam
{

	Window::Window()
		: p_State(Global::GlobalWindowState), p_Window(nullptr)
	{
	}

	void Window::Init()
	{
		glfwInit();

		glfwWindowHint(GLFW_DECORATED, p_State.TitleBar);

		auto& contextVersion = Global::GlobaOpenGLVersion;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextVersion.Major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextVersion.Minor);
		glfwWindowHint(GLFW_CONTEXT_REVISION, contextVersion.Revision);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		p_Window = glfwCreateWindow(p_State.Width, p_State.Height, p_State.Title.c_str(), NULL, NULL);

		if (p_Window == NULL)
		{
			glfwTerminate();
			JAM_ASSERT(false, "window initialization failed");
			return;
		}

		glfwMakeContextCurrent(p_Window);

		SetVSync(p_State.VSync);

		InitCallbacks();

		std::function<void(WindowResizeEvent*)>* f_WindowResizeEvent = new std::function<void(WindowResizeEvent*)>([&](WindowResizeEvent* f_E) {
			p_State.Width = f_E->GetXSize();
			p_State.Height = f_E->GetYSize();
			});
		Publisher<WindowResizeEvent*>::Subscribe(f_WindowResizeEvent);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(p_Window);
	}

	void Window::Update()
	{
		glfwSwapBuffers(p_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(bool f_Enabled)
	{
		if (f_Enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}

	int Window::IsIconified()
	{
		return glfwGetWindowAttrib(p_Window, GLFW_ICONIFIED);
	}

	void Window::CaptureCursor(bool f_Enabled)
	{
		if (f_Enabled)
		{
			glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(p_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void Window::InitCallbacks()
	{
		glfwSetWindowIconifyCallback(p_Window, [](GLFWwindow* window, int icon)
			{
				if (icon)
				{
					WindowMinimizeEvent* e = new WindowMinimizeEvent();
					EventQueue::Push(e);
				}
				else
				{
					WindowUnminimizeEvent* e = new WindowUnminimizeEvent();
					EventQueue::Push(e);
				}
			});

		glfwSetWindowSizeCallback(p_Window, [](GLFWwindow* window, int width, int height)
			{
				if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) return;
				WindowResizeEvent* e = new WindowResizeEvent(width, height);
				EventQueue::Push(e);
			});

		glfwSetWindowCloseCallback(p_Window, [](GLFWwindow* window)
			{
				WindowCloseEvent* e = new WindowCloseEvent();
				EventQueue::Push(e);
			});

		glfwSetWindowFocusCallback(p_Window, [](GLFWwindow* window, int focused)
			{
				switch (focused)
				{
				case GLFW_TRUE:
				{
					WindowFocusEvent* e = new WindowFocusEvent();
					EventQueue::Push(e);
					break;
				}
				case GLFW_FALSE:
				{
					WindowLostFocusEvent* e = new WindowLostFocusEvent();
					EventQueue::Push(e);
					break;
				}
				}
			});

		glfwSetWindowPosCallback(p_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowMoveEvent* e = new WindowMoveEvent(xpos, ypos);
				EventQueue::Push(e);
			});
	}

}