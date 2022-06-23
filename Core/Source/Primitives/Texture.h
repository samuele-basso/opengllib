#pragma once

#include <glad/glad.h>

#include <vector>

#include <glm/glm.hpp>
#include <string>


namespace Jam
{

	enum class TextureParameter
	{
		WrapS, WrapT, WrapR,
		FilterMin, FilterMag
	};

	enum class TextureParameterValue
	{
		ClampToEdge, Repeat,
		Nearest, Linear
	};

	class Texture
	{
	public:
		Texture(std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters);
		virtual ~Texture();
		Texture(const Texture&) = default;

		unsigned int GetID() { return p_ID; }
		void Bind(int f_Slot);

		int GetWidth() { return p_Width; }
		int GetHeight() { return p_Height; }

	protected:
		void SetFormat(int f_Channels);
		void ApplyParameters();

	protected:
		unsigned int p_ID;
		int p_Width, p_Height;

		GLenum p_Format;
		GLenum p_InternalFormat;
		GLenum p_Target;

		std::vector<std::tuple<TextureParameter, TextureParameterValue>> p_Parameters;
	};

	enum class TextureInternalFormat
	{
		None, RGB8, RGBA8, RGBA16F, RGBA32F, R8
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(
			std::initializer_list<std::tuple<TextureParameter,
			TextureParameterValue>> f_Parameters,
			const glm::vec4& f_Color
		);
		Texture2D(
			std::initializer_list<std::tuple<TextureParameter,
			TextureParameterValue>> f_Parameters,
			const std::string& f_Path
		);
		Texture2D(
			std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
			int f_Width,
			int f_Height,
			int f_Channels,
			void* f_Data
		);
		Texture2D(
			std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
			int f_Width,
			int f_Height,
			TextureInternalFormat f_InternalFormat
		);
		Texture2D(
			std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
			int f_Width,
			int f_Height,
			TextureInternalFormat f_InternalFormat,
			void* f_Data
		);
		virtual ~Texture2D() = default;
		Texture2D(const Texture2D&) = default;
	};

	class Texture2DMultisample : public Texture
	{
	public:
		Texture2DMultisample(
			std::initializer_list<std::tuple<TextureParameter,
			TextureParameterValue>> f_Parameters,
			int f_Width,
			int f_Height,
			TextureInternalFormat f_InternalFormat,
			int f_Sandboxs
		);
		virtual ~Texture2DMultisample() = default;
		Texture2DMultisample(const Texture2DMultisample&) = default;

	private:
		int p_Sandboxs;
	};

}