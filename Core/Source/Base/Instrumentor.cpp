#include "gfxpch.h"
#include "Instrumentor.h"

namespace Jam
{

	void Instrumentor::BeginSession(const std::string& f_Name, const std::string& f_Filepath)
	{
		std::lock_guard lock(p_Mutex);
		if (p_CurrentSession)
		{
			JAM_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", f_Name, p_CurrentSession->Name);
			InternalEndSession();
		}
		p_OutputStream.open(f_Filepath);

		if (p_OutputStream.is_open())
		{
			p_CurrentSession = new InstrumentationSession({ f_Name });
			WriteHeader();
		}
		else
		{
			JAM_CORE_ERROR("instrumentor could not open results file '{0}'", f_Filepath);
		}
	}

	void Instrumentor::EndSession()
	{
		std::lock_guard lock(p_Mutex);
		InternalEndSession();
	}

	void Instrumentor::WriteProfile(const ProfileResult& f_Result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (f_Result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << f_Result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << f_Result.ThreadID << ",";
		json << "\"ts\":" << f_Result.Start.count();
		json << "}";

		std::lock_guard lock(p_Mutex);
		if (p_CurrentSession)
		{
			p_OutputStream << json.str();
			p_OutputStream.flush();
		}
	}

	Instrumentor& Instrumentor::Get()
	{
		static Instrumentor instance;
		return instance;
	}

	Instrumentor::Instrumentor()
		: p_CurrentSession(nullptr)
	{
	}

	Instrumentor::~Instrumentor()
	{
		EndSession();
	}

	void Instrumentor::WriteHeader()
	{
		p_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		p_OutputStream.flush();
	}

	void Instrumentor::WriteFooter()
	{
		p_OutputStream << "]}";
		p_OutputStream.flush();
	}

	void Instrumentor::InternalEndSession()
	{
		if (p_CurrentSession)
		{
			WriteFooter();
			p_OutputStream.close();
			delete p_CurrentSession;
			p_CurrentSession = nullptr;
		}
	}

	InstrumentationTimer::InstrumentationTimer(const char* f_Name)
		: p_Name(f_Name), p_Stopped(false)
	{
		p_StartTimepoint = std::chrono::steady_clock::now();
	}

	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!p_Stopped)
			Stop();
	}

	void InstrumentationTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{ p_StartTimepoint.time_since_epoch() };
		auto elapsedTime =
			std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
			std::chrono::time_point_cast<std::chrono::microseconds>(p_StartTimepoint).time_since_epoch();

		Instrumentor::Get().WriteProfile({ p_Name, highResStart, elapsedTime, std::this_thread::get_id() });

		p_Stopped = true;
	}

}