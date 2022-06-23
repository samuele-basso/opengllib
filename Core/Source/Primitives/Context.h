#pragma once

#include "Base/Window.h"

#include <glad/glad.h>

namespace Jam
{

	enum class OpenGLLogLevel
	{
		notification = 0,
		low = 1,
		medium = 2,
		high = 3,
		highAssert = 4,
		none = 5
	};

	void OpenGLLogMessage(GLenum f_Source, GLenum f_Type, GLuint f_ID, GLenum f_Severity, GLsizei f_Length, const GLchar* f_Message, const void* f_UserParam);
	void OpenGLLogEnable();
	void OpenGLLogLevelSet(OpenGLLogLevel f_Level);

	class Context
	{
	public:
		Context() = default;
		~Context() = default;
		Context(const Context&) = delete;

		virtual void Init(Window* f_Window);

	public:
		static void SetClearColor(glm::vec4& f_Color);
		static void Blend(bool f_State);
		static void BackFaceCulling(bool f_State);
		static void Depth(bool f_State);
	};

}