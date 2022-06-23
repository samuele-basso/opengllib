/*
Credits:
	https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h
*/

#pragma once

#include <fstream>

#include "Base/Log.h"

namespace Jam
{

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& f_Name, const std::string& f_Filepath = "results.json");
		void EndSession();

		void WriteProfile(const ProfileResult& f_Result);

		static Instrumentor& Get();

	private:
		Instrumentor();
		~Instrumentor();

		void WriteHeader();
		void WriteFooter();

		void InternalEndSession();

	private:
		std::mutex p_Mutex;
		InstrumentationSession* p_CurrentSession;
		std::ofstream p_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* f_Name);
		~InstrumentationTimer();

		void Stop();

	private:
		const char* p_Name;
		std::chrono::time_point<std::chrono::steady_clock> p_StartTimepoint;
		bool p_Stopped;
	};

	namespace InstrumentorUtils
	{

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&f_Expr)[N], const char(&f_Remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && f_Expr[srcIndex + matchIndex] == f_Remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = f_Expr[srcIndex] == '"' ? '\'' : f_Expr[srcIndex];
				srcIndex++;
			}
			return result;
		}

	}

}

#if JAM_DEBUG_ENABLED
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define JAM_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define JAM_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define JAM_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define JAM_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define JAM_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define JAM_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define JAM_FUNC_SIG __func__
#else
#define JAM_FUNC_SIG "JAM_FUNC_SIG unknown!"
#endif

#define JAM_PROFILE_BEGIN_SESSION(name, filepath) ::Jam::Instrumentor::Get().BeginSession(name, filepath)
#define JAM_PROFILE_END_SESSION() ::Jam::Instrumentor::Get().EndSession()
#define JAM_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Jam::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::Jam::InstrumentationTimer timer##line(fixedName##line.Data)
#define JAM_PROFILE_SCOPE_LINE(name, line) JAM_PROFILE_SCOPE_LINE2(name, line)
#define JAM_PROFILE_SCOPE(name) JAM_PROFILE_SCOPE_LINE(name, __LINE__)
#define JAM_PROFILE_FUNCTION() JAM_PROFILE_SCOPE(JAM_FUNC_SIG)
#else
#define JAM_PROFILE_BEGIN_SESSION(name, filepath)
#define JAM_PROFILE_END_SESSION()
#define JAM_PROFILE_SCOPE(name)
#define JAM_PROFILE_FUNCTION()
#endif