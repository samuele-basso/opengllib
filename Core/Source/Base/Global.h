#pragma once

#include <glm/glm.hpp>

namespace Jam
{

	class Global
	{
	public:
		Global() = delete;
		~Global() = delete;
		Global(const Global&) = delete;

	public:
		static struct ViewportState
		{
			ViewportState(uint32_t f_Width = 600, uint32_t f_Height = 600, bool f_Valid = false)
				: Width(f_Width), Height(f_Height), Valid(f_Valid)
			{
			}

			uint32_t Width;
			uint32_t Height;

			bool Valid;
		} GlobalViewportState;

		static struct WindowState
		{
			WindowState(bool f_TitleBar = true, const std::string& f_Title = "Jam", uint32_t f_Width = 1450, uint32_t f_Height = 1050, bool f_VSync = false)
				: TitleBar(f_TitleBar), Title(f_Title), Width(f_Width), Height(f_Height), VSync(f_VSync)
			{
			}

			bool TitleBar;
			std::string Title;

			uint32_t Width;
			uint32_t Height;

			bool VSync;
		} GlobalWindowState;

		static struct OpenGLVersion
		{
			OpenGLVersion(short f_Major = 4, short f_Minor = 6, short f_Revision = 0)
				: Major(f_Major), Minor(f_Minor), Revision(f_Revision)
			{
			}

			short Major;
			short Minor;
			short Revision;
		} GlobaOpenGLVersion;

		static void Init();
	};

}