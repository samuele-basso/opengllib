#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <memory>

namespace Jam
{

	class Logger
	{
	private:
		static struct Sinks
		{
			Sinks()
			{
				List.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				List.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("jam.log", true));

				List[0]->set_pattern("%^[%T] %n: %v%$");
				List[1]->set_pattern("[%T] [%l] %n: %v");
			}

			std::vector<spdlog::sink_ptr> List;

		} p_Sinks;

	public:
		Logger(const char* f_Name);
		virtual ~Logger() = default;
		Logger(const Logger&) = default;

		std::shared_ptr<spdlog::logger>& GetLogger() { return p_Logger; }

	protected:
		std::shared_ptr<spdlog::logger> p_Logger;
	};

	class Core : public Jam::Logger
	{
	public:
		Core()
			: Logger("Core")
		{
		}

		static Core Instance;
	};

	class General : public Jam::Logger
	{
	public:
		General()
			: Logger("General")
		{
		}

		static General Instance;
	};
}

#ifdef JAM_DEBUG_ENABLED

#define JAM_CORE_TRACE(...)    ::Jam::Core::Instance.GetLogger()->trace(__VA_ARGS__)
#define JAM_CORE_INFO(...)     ::Jam::Core::Instance.GetLogger()->info(__VA_ARGS__)
#define JAM_CORE_WARN(...)     ::Jam::Core::Instance.GetLogger()->warn(__VA_ARGS__)
#define JAM_CORE_ERROR(...)    ::Jam::Core::Instance.GetLogger()->error(__VA_ARGS__)
#define JAM_CORE_CRITICAL(...) ::Jam::Core::Instance.GetLogger()->critical(__VA_ARGS__)

#define JAM_TRACE(...)    ::Jam::General::Instance.GetLogger()->trace(__VA_ARGS__)
#define JAM_INFO(...)     ::Jam::General::Instance.GetLogger()->info(__VA_ARGS__)
#define JAM_WARN(...)     ::Jam::General::Instance.GetLogger()->warn(__VA_ARGS__)
#define JAM_ERROR(...)    ::Jam::General::Instance.GetLogger()->error(__VA_ARGS__)
#define JAM_CRITICAL(...) ::Jam::General::Instance.GetLogger()->critical(__VA_ARGS__)

#define JAM_ASSERT(condition, message) assert(condition && message)

#else

#define JAM_CORE_TRACE
#define JAM_CORE_INFO
#define JAM_CORE_WARN
#define JAM_CORE_ERROR
#define JAM_CORE_CRITICAL

#define JAM_TRACE
#define JAM_INFO
#define JAM_WARN
#define JAM_ERROR
#define JAM_CRITICAL

#define JAM_ASSERT(condition, message)

#endif