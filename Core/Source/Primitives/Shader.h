#pragma once

#include "Context.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace Jam
{

	class Stage
	{
	public:
		Stage(std::string f_Path, std::initializer_list<std::string> f_Prepend = { })
			: Path(f_Path), Prepend(f_Prepend)
		{
		}

		std::string Path;
		std::vector<std::string> Prepend;
	};

	enum class Constant
	{
		VERTEX,
		GEOMETRY,
		FRAGMENT
	};

	class Shader
	{
	public:
		Shader();

		void Init(const Stage& f_StageVS, const Stage& f_StageFS);
		void Init(const Stage& f_StageVS, const Stage& f_StageGS, const Stage& f_StageFS);

		~Shader();
		Shader(const Shader&) = default;

		void Bind();
		void Unbind();

		void SetUniform1i(const std::string& f_Name, const int f_Value);

		void SetUniform1iv(const std::string& f_Name, int f_Count, const int* f_Value);

		void SetUniform1uiv(const std::string& f_Name, int f_Count, const unsigned int* f_Value);

		void SetUniform1fv(const std::string& f_Name, int f_Count, const float* f_Value);
		void SetUniform2fv(const std::string& f_Name, int f_Count, const float* f_Value);
		void SetUniform3fv(const std::string& f_Name, int f_Count, const float* f_Value);
		void SetUniform4fv(const std::string& f_Name, int f_Count, const float* f_Value);

		void SetUniformMatrix4fv(const std::string& f_Name, int f_Count, unsigned char f_Transpose, const float* f_Value);

		unsigned int GetID() { return p_ID; }

		void SetSubroutine(Constant f_Stage, std::string& f_Uniform, std::string& f_Bind);
		void SubroutineFlush();

	private:
		std::unordered_map<std::string, int> p_Uniform;
		int GetUniformLocation(const std::string& f_Name);

		std::tuple<std::string, std::string> LoadShader(const Stage& f_StageVS, const Stage& f_StageFS);
		std::tuple<std::string, std::string, std::string> LoadShader(const Stage& f_StageVS, const Stage& f_StageGS, const Stage& f_StageFS);

		unsigned int CompileShader(unsigned int f_Type, const std::string& f_Source);

		void CreateProgram(const std::string& f_SourceVS, const std::string& f_SourceFS);
		void CreateProgram(const std::string& f_SourceVS, const std::string& f_SourceGS, const std::string& f_SourceFS);

		unsigned int p_ID;

		int p_SubroutineCountVertex;
		std::vector<unsigned int> p_VertexSubroutines;

		int p_SubroutineCountGeometry;
		std::vector<unsigned int> p_GeometrySubroutines;

		int p_SubroutineCountFragment;
		std::vector<unsigned int> p_FragmentSubroutines;
	};

}