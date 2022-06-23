#include "gfxpch.h"
#include "Context.h"

#include "Events/WindowEvent.h"
#include "Events/EventSys.h"
#include "Base/Log.h"

namespace Jam
{

	void OpenGLLogEnable()
	{
		glDebugMessageCallback(OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	static OpenGLLogLevel s_OpenGLLogLevel = OpenGLLogLevel::highAssert;

	void OpenGLLogLevelSet(OpenGLLogLevel f_Level)
	{
		s_OpenGLLogLevel = f_Level;
	}

	void OpenGLLogMessage(GLenum f_Source, GLenum f_Type, GLuint f_ID, GLenum f_Severity, GLsizei f_Length, const GLchar* f_Message, const void* f_UserParam)
	{
		switch (f_Severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			if ((int)s_OpenGLLogLevel < 5)
			{
				JAM_CORE_ERROR("[OpenGL Debug HIGH] {0}", f_Message);
				if (s_OpenGLLogLevel == OpenGLLogLevel::highAssert)
				{
					JAM_ASSERT(false, "OpenGL Debug HIGH");
				}
			}
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			if ((int)s_OpenGLLogLevel < 3)
			{
				JAM_CORE_WARN("[OpenGL Debug MEDIUM] {0}", f_Message);
			}
			break;
		case GL_DEBUG_SEVERITY_LOW:
			if ((int)s_OpenGLLogLevel < 2)
			{
				JAM_CORE_INFO("[OpenGL Debug LOW] {0}", f_Message);
			}
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			if ((int)s_OpenGLLogLevel < 1)
			{
				JAM_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", f_Message);
			}
			break;
		}
	}

	void Context::Init(Window* f_Window)
	{
		glfwMakeContextCurrent(f_Window->GetGLFWWindow());
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		JAM_ASSERT(status, "glad load fail");

		std::function<void(ViewportResizeEvent*)>* viewportResizeEvent = new std::function<void(ViewportResizeEvent*)>([&](ViewportResizeEvent* f_E) {
			glViewport(0.0f, 0.0f, f_E->GetXSize(), f_E->GetYSize());
			});
		Publisher<ViewportResizeEvent*>::Subscribe(viewportResizeEvent);

#if JAM_DEBUG_ENABLED
		OpenGLLogEnable();
#endif
	}

	void Context::SetClearColor(glm::vec4& f_Color)
	{
		glClearColor(
			f_Color.r, f_Color.g, f_Color.b, f_Color.a
		);
	}

	void Context::Blend(bool f_State)
	{
		if (f_State)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void Context::BackFaceCulling(bool f_State)
	{
		if (f_State)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	void Context::Depth(bool f_State)
	{
		if (f_State)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

}