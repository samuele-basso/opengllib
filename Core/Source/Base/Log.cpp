#include "gfxpch.h"
#include "Log.h"

namespace Jam
{

	Logger::Sinks Logger::p_Sinks;

	Logger::Logger::Logger(const char* f_Name)
	{
		p_Logger = std::make_shared<spdlog::logger>(f_Name, begin(p_Sinks.List), end(p_Sinks.List));
		spdlog::register_logger(p_Logger);
		p_Logger->set_level(spdlog::level::trace);
		p_Logger->flush_on(spdlog::level::trace);
	}

	Core Core::Instance;
	General General::Instance;

}