#include "gfxpch.h"
#include "Shader.h"

#include "Base/Log.h"

#include "Base/Global.h"

namespace Jam
{
	Shader::Shader()
		: p_ID(0), p_SubroutineCountVertex(0), p_SubroutineCountGeometry(0), p_SubroutineCountFragment(0)
	{
	}

	void Shader::Init(const Stage& f_StageVS, const Stage& f_StageFS)
	{
		auto [v, f] = LoadShader(f_StageVS, f_StageFS);

		CreateProgram(v, f);

		glGetProgramStageiv(p_ID, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &p_SubroutineCountVertex);
		p_VertexSubroutines = std::vector<unsigned int>(p_SubroutineCountVertex);

		glGetProgramStageiv(p_ID, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &p_SubroutineCountFragment);
		p_FragmentSubroutines = std::vector<unsigned int>(p_SubroutineCountFragment);
	}

	void Shader::Init(const Stage& f_StageVS, const Stage& f_StageGS, const Stage& f_StageFS)
	{
		auto [v, g, f] = LoadShader(f_StageVS, f_StageGS, f_StageFS);

		CreateProgram(v, g, f);

		glGetProgramStageiv(p_ID, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &p_SubroutineCountVertex);
		p_VertexSubroutines = std::vector<unsigned int>(p_SubroutineCountVertex);

		glGetProgramStageiv(p_ID, GL_GEOMETRY_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &p_SubroutineCountGeometry);
		p_GeometrySubroutines = std::vector<unsigned int>(p_SubroutineCountGeometry);

		glGetProgramStageiv(p_ID, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &p_SubroutineCountFragment);
		p_FragmentSubroutines = std::vector<unsigned int>(p_SubroutineCountFragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(p_ID);
	}

	void Shader::Bind()
	{
		glUseProgram(p_ID);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1i(const std::string& f_Name, const int f_Value)
	{
		glUniform1i(GetUniformLocation(f_Name), f_Value);
	}

	void Shader::SetUniform1iv(const std::string& f_Name, int f_Count, const int* f_Value)
	{
		glUniform1iv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniform1uiv(const std::string& f_Name, int f_Count, const unsigned int* f_Value)
	{
		glUniform1uiv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniform1fv(const std::string& f_Name, int f_Count, const float* f_Value)
	{
		glUniform1fv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniform2fv(const std::string& f_Name, int f_Count, const float* f_Value)
	{
		glUniform2fv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniform3fv(const std::string& f_Name, int f_Count, const float* f_Value)
	{
		glUniform3fv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniform4fv(const std::string& f_Name, int f_Count, const float* f_Value)
	{
		glUniform4fv(GetUniformLocation(f_Name), f_Count, f_Value);
	}

	void Shader::SetUniformMatrix4fv(const std::string& f_Name, int f_Count, unsigned char f_Transpose, const float* f_Value)
	{
		glUniformMatrix4fv(GetUniformLocation(f_Name), f_Count, f_Transpose, f_Value);
	}

	void Shader::SetSubroutine(Constant f_Stage, std::string& f_Uniform, std::string& f_Bind)
	{
		switch (f_Stage)
		{
		case Constant::VERTEX:
			p_VertexSubroutines[glGetSubroutineUniformLocation(p_ID, GL_VERTEX_SHADER, f_Uniform.c_str())] =
				glGetSubroutineIndex(p_ID, GL_VERTEX_SHADER, f_Bind.c_str());
			break;

		case Constant::GEOMETRY:
			p_GeometrySubroutines[glGetSubroutineUniformLocation(p_ID, GL_GEOMETRY_SHADER, f_Uniform.c_str())] =
				glGetSubroutineIndex(p_ID, GL_GEOMETRY_SHADER, f_Bind.c_str());
			break;

		case Constant::FRAGMENT:
			p_FragmentSubroutines[glGetSubroutineUniformLocation(p_ID, GL_FRAGMENT_SHADER, f_Uniform.c_str())] =
				glGetSubroutineIndex(p_ID, GL_FRAGMENT_SHADER, f_Bind.c_str());
			break;
		}
	}

	void Shader::SubroutineFlush()
	{
		if (p_SubroutineCountVertex > 0)
			glUniformSubroutinesuiv(GL_VERTEX_SHADER, p_SubroutineCountVertex, p_VertexSubroutines.data());

		if (p_SubroutineCountGeometry > 0)
			glUniformSubroutinesuiv(GL_GEOMETRY_SHADER, p_SubroutineCountGeometry, p_GeometrySubroutines.data());

		if (p_SubroutineCountFragment > 0)
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, p_SubroutineCountFragment, p_FragmentSubroutines.data());
	}

	int Shader::GetUniformLocation(const std::string& f_Name)
	{
		auto location = p_Uniform.find(f_Name);
		if (location != p_Uniform.end())
			return location->second;
		int newLocation = glGetUniformLocation(p_ID, f_Name.c_str());
		if (newLocation != -1)
		{
			p_Uniform.insert(std::make_pair(f_Name, newLocation));
			return newLocation;
		}
		return 0;
	}

	std::tuple<std::string, std::string> Shader::LoadShader(const Stage& f_StageVS, const Stage& f_StageFS)
	{
		std::fstream in;

		auto& contextVersion = Global::GlobaOpenGLVersion;

		in.open(f_StageVS.Path);
		std::stringstream vs;
		vs << "#version " << contextVersion.Major << contextVersion.Minor << contextVersion.Revision << " core\n";
		for (auto& prepend : f_StageVS.Prepend) vs << "#define " << prepend << "\n";
		vs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		in.clear();

		in.open(f_StageFS.Path);
		std::stringstream fs;
		fs << "#version " << contextVersion.Major << contextVersion.Minor << contextVersion.Revision << " core\n";
		for (auto& prepend : f_StageFS.Prepend) fs << "#define " << prepend << "\n";
		fs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		in.clear();

		return { vs.str(), fs.str() };
	}

	std::tuple<std::string, std::string, std::string> Shader::LoadShader(const Stage& f_StageVS, const Stage& f_StageGS, const Stage& f_StageFS)
	{
		std::fstream in;

		auto& contextVersion = Global::GlobaOpenGLVersion;

		in.open(f_StageVS.Path);
		std::stringstream vs;
		vs << "#version " << contextVersion.Major << contextVersion.Minor << contextVersion.Revision << " core\n";
		for (auto& prepend : f_StageVS.Prepend) vs << "#define " << prepend << "\n";
		vs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		in.clear();

		in.open(f_StageGS.Path);
		std::stringstream gs;
		gs << "#version " << contextVersion.Major << contextVersion.Minor << contextVersion.Revision << " core\n";
		for (auto& prepend : f_StageGS.Prepend) gs << "#define " << prepend << "\n";
		gs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		in.clear();

		in.open(f_StageFS.Path);
		std::stringstream fs;
		fs << "#version " << contextVersion.Major << contextVersion.Minor << contextVersion.Revision << " core\n";
		for (auto& prepend : f_StageFS.Prepend) fs << "#define " << prepend << "\n";
		fs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();
		in.clear();

		return { vs.str(), gs.str(), fs.str() };
	}

	unsigned int Shader::CompileShader(unsigned int f_Type, const std::string& f_Source)
	{
		unsigned int ID = glCreateShader(f_Type);
		const char* c_src = f_Source.c_str();
		glShaderSource(ID, 1, &c_src, nullptr);
		glCompileShader(ID);

		int result;
		glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_malloca(length * sizeof(char));

			glGetShaderInfoLog(ID, length, &length, message);

			JAM_CORE_CRITICAL(message);

			glDeleteShader(ID);
			return 0;
		}

		return ID;
	}

	void Shader::CreateProgram(const std::string& f_SourceVS, const std::string& f_SourceFS)
	{
		p_ID = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, f_SourceVS);
		JAM_ASSERT(vs, "vertex shader compilation failed");

		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, f_SourceFS);
		JAM_ASSERT(fs, "fragment shader compilation failed");

		glAttachShader(p_ID, vs);
		glAttachShader(p_ID, fs);

		glLinkProgram(p_ID);

		glValidateProgram(p_ID);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void Shader::CreateProgram(const std::string& f_SourceVS, const std::string& f_SourceGS, const std::string& f_SourceFS)
	{
		p_ID = glCreateProgram();

		unsigned int vs = CompileShader(GL_VERTEX_SHADER, f_SourceVS);
		JAM_ASSERT(vs, "vertex shader compilation failed");

		unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, f_SourceGS);
		JAM_ASSERT(gs, "geometry shader compilation failed");

		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, f_SourceFS);
		JAM_ASSERT(fs, "fragment shader compilation failed");

		glAttachShader(p_ID, vs);
		glAttachShader(p_ID, gs);
		glAttachShader(p_ID, fs);

		glLinkProgram(p_ID);

		glValidateProgram(p_ID);

		int result;
		glGetProgramiv(p_ID, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			JAM_ASSERT(false, "shader link failed");
		}

		glDeleteShader(vs);
		glDeleteShader(gs);
		glDeleteShader(fs);
	}

}