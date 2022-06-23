#pragma once

#include <string>

#include "Window.h"

namespace Jam
{

	class FileDialog
	{
	public:
		FileDialog() = delete;
		~FileDialog() = delete;
		FileDialog(const FileDialog&) = delete;

	public:
		static std::string Open(bool* f_Success, const char* f_Filter, Window* f_Window);
	};

}