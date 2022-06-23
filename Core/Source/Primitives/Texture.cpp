#include "gfxpch.h"
#include "Texture.h"

#include "Base/Log.h"

namespace Jam
{

	Texture::Texture(std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters)
		: p_ID(0), p_Width(0), p_Height(0), p_Format(0), p_InternalFormat(0), p_Target(0), p_Parameters(f_Parameters)
	{
	}

	void Texture::Bind(int f_Slot)
	{
		glBindTextureUnit(f_Slot, p_ID);
	}

	static GLenum TextureParameterType(TextureParameter f_Parameter)
	{
		switch (f_Parameter)
		{
		case TextureParameter::WrapS:     return GL_TEXTURE_WRAP_S;
		case TextureParameter::WrapT:     return GL_TEXTURE_WRAP_T;
		case TextureParameter::WrapR:     return GL_TEXTURE_WRAP_R;
		case TextureParameter::FilterMin: return GL_TEXTURE_MIN_FILTER;
		case TextureParameter::FilterMag: return GL_TEXTURE_MAG_FILTER;
		}
		return 0;
	}

	static GLenum TextureParameterTypeValue(TextureParameterValue f_Value)
	{
		switch (f_Value)
		{
		case TextureParameterValue::Nearest:     return GL_NEAREST;
		case TextureParameterValue::Linear:      return GL_LINEAR;
		case TextureParameterValue::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureParameterValue::Repeat:      return GL_REPEAT;
		}
		return 0;
	}

	void Texture::ApplyParameters()
	{
		for (std::tuple<TextureParameter, TextureParameterValue> p : p_Parameters)
		{
			glTextureParameteri(p_ID,
				TextureParameterType(std::get<0>(p)),
				TextureParameterTypeValue(std::get<1>(p))
			);
		}
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &p_ID);
	}

	void Texture::SetFormat(int f_Channels)
	{
		if (f_Channels == 1)
		{
			p_Format = GL_RED;
			p_InternalFormat = GL_R8;
			return;
		}

		if (f_Channels == 2)
		{
			p_Format = GL_RG;
			p_InternalFormat = GL_RG8;
			return;
		}

		if (f_Channels == 3)
		{
			p_Format = GL_RGB;
			p_InternalFormat = GL_RGB8;
			return;
		}

		if (f_Channels == 4)
		{
			p_Format = GL_RGBA;
			p_InternalFormat = GL_RGBA8;
			return;
		}

		JAM_ASSERT(false, "format error");
	}

	Texture2D::Texture2D(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		const glm::vec4& f_Color
	)
		: Texture(f_Parameters)
	{
		p_Width = 1;
		p_Height = 1;
		p_Format = GL_RGBA;
		p_InternalFormat = GL_RGBA8;

		glCreateTextures(GL_TEXTURE_2D, 1, &p_ID);
		glTextureStorage2D(p_ID, 1, p_InternalFormat, p_Width, p_Height);

		ApplyParameters();

		glTextureSubImage2D(p_ID, 0, 0, 0, p_Width, p_Height, p_Format, GL_FLOAT, glm::value_ptr(f_Color));
		glGenerateTextureMipmap(p_ID);
	}

	Texture2D::Texture2D(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		const std::string& f_Path
	)
		: Texture(f_Parameters)
	{
		stbi_set_flip_vertically_on_load(1);

		int channels = 0;

		stbi_uc* image = nullptr;
		image = stbi_load(f_Path.c_str(), &p_Width, &p_Height, &channels, 0);

		if (image)
		{
			SetFormat(channels);

			glCreateTextures(GL_TEXTURE_2D, 1, &p_ID);
			glTextureStorage2D(p_ID, 1, p_InternalFormat, p_Width, p_Height);

			ApplyParameters();

			glTextureSubImage2D(p_ID, 0, 0, 0, p_Width, p_Height, p_Format, GL_UNSIGNED_BYTE, image);
			glGenerateTextureMipmap(p_ID);

			stbi_image_free(image);
		}
	}

	Texture2D::Texture2D(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		int f_Width,
		int f_Height,
		int f_Channels,
		void* f_Data
	)
		: Texture(f_Parameters)
	{
		p_Width = f_Width;
		p_Height = f_Height;

		SetFormat(f_Channels);

		glCreateTextures(GL_TEXTURE_2D, 1, &p_ID);
		glTextureStorage2D(p_ID, 1, p_InternalFormat, p_Width, p_Height);

		ApplyParameters();

		glTextureSubImage2D(p_ID, 0, 0, 0, p_Width, p_Height, p_Format, GL_UNSIGNED_BYTE, f_Data);
		glGenerateTextureMipmap(p_ID);
	}

	static GLenum GetInternalFormat(TextureInternalFormat f_Value)
	{
		switch (f_Value)
		{
		case TextureInternalFormat::R8:       return GL_R8_SNORM;
		case TextureInternalFormat::RGB8:     return GL_RGB8;
		case TextureInternalFormat::RGBA8:    return GL_RGBA8;
		case TextureInternalFormat::RGBA16F:  return GL_RGBA16F;
		case TextureInternalFormat::RGBA32F:  return GL_RGBA32F;
		}
		return 0;
	}

	Texture2D::Texture2D(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		int f_Width,
		int f_Height,
		TextureInternalFormat f_InternalFormat
	)
		: Texture(f_Parameters)
	{
		p_Width = f_Width;
		p_Height = f_Height;

		p_Format = p_InternalFormat = GetInternalFormat(f_InternalFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &p_ID);
		glTextureStorage2D(p_ID, 1, p_InternalFormat, p_Width, p_Height);

		ApplyParameters();
	}

	Texture2D::Texture2D(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		int f_Width,
		int f_Height,
		TextureInternalFormat f_InternalFormat,
		void* f_Data
	)
		: Texture(f_Parameters)
	{
		p_Width = f_Width;
		p_Height = f_Height;

		p_Format = p_InternalFormat = GetInternalFormat(f_InternalFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &p_ID);
		glTextureStorage2D(p_ID, 1, p_InternalFormat, p_Width, p_Height);

		ApplyParameters();

		glTextureSubImage2D(p_ID, 0, 0, 0, p_Width, p_Height, GL_RGBA, GL_FLOAT, f_Data);
	}


	Texture2DMultisample::Texture2DMultisample(
		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> f_Parameters,
		int f_Width,
		int f_Height,
		TextureInternalFormat f_InternalFormat,
		int f_Sandboxs
	)
		: Texture(f_Parameters), p_Sandboxs(0)
	{
		p_Width = f_Width;
		p_Height = f_Height;

		p_Format = p_InternalFormat = GetInternalFormat(f_InternalFormat);

		p_Sandboxs = f_Sandboxs;

		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &p_ID);
		glTextureStorage2DMultisample(p_ID, p_Sandboxs, p_InternalFormat, p_Width, p_Height, true);

		ApplyParameters();
	}

}