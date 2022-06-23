#pragma once

#include "Base/Log.h"
#include "Texture.h"

namespace Jam
{

	template<unsigned int N>
	class Framebuffer
	{
	public:
		Framebuffer();

		void Init(std::initializer_list<TextureInternalFormat> f_Attachments, int f_Width, int f_Height, bool f_IsMultiSandbox = false, int f_Sandboxs = 4);

		~Framebuffer();
		Framebuffer(const Framebuffer<N>&) = default;

		void Clear();

		void Bind();
		void Unbind();

		void BindRead();

		std::vector<Texture*>& GetTextures() { return p_Textures; }

		void GenAttachments(int f_Width, int f_Height);
		void UpdateSandboxs(int f_Sandboxs);

		void EnableMultiSampling(int f_Sandboxs);
		void DisableMultiSampling();

		int GetWidth() { return p_Width; }
		int GetHeight() { return p_Height; }

	private:
		void GenAttachmentsInternal(int f_Width, int f_Height);

	private:
		std::vector<TextureInternalFormat> p_Attachments;
		std::vector<Texture*> p_Textures{ N, nullptr };

		unsigned int p_RenderBuffer;
		unsigned int p_ID;

		bool p_IsMultiSandbox;
		int p_Sandboxs;

		int p_Width;
		int p_Height;

		GLenum p_ColorAttachments[N];
	};

	template<unsigned int N>
	Framebuffer<N>::Framebuffer()
		: p_ID(0), p_RenderBuffer(0), p_Sandboxs(0), p_IsMultiSandbox(false), p_Width(0), p_Height(0), p_Attachments( {} )
	{
	}

	template<unsigned int N>
	inline void Framebuffer<N>::Init(std::initializer_list<TextureInternalFormat> f_Attachments, int f_Width, int f_Height, bool f_IsMultiSandbox, int f_Sandboxs)
	{
		GLint supportedAttachments = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &supportedAttachments);

		GLint supportedDrawBuffers = 0;
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &supportedDrawBuffers);

		JAM_ASSERT(supportedAttachments > N && supportedDrawBuffers > N, "invalid attachments count");

		for (int i = 0; i < N; i++)
		{
			p_ColorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		p_Attachments = f_Attachments;

		glGenFramebuffers(1, &p_ID);

		GenAttachmentsInternal(f_Width, f_Height);

		glBindFramebuffer(GL_FRAMEBUFFER, p_ID);

		JAM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "invalid framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	template<unsigned int N>
	Framebuffer<N>::~Framebuffer()
	{
		Clear();
		glDeleteFramebuffers(1, &p_ID);
	}

	template<unsigned int N>
	void Framebuffer<N>::Clear()
	{
		glDeleteRenderbuffers(1, &p_RenderBuffer);
		for (int i = 0; i < N; i++)
		{
			delete p_Textures[i];
		}
	}

	template<unsigned int N>
	void Framebuffer<N>::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, p_ID);
	}

	template<unsigned int N>
	void Framebuffer<N>::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	template<unsigned int N>
	void Framebuffer<N>::BindRead()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, p_ID);
	}

	template<unsigned int N>
	inline void Framebuffer<N>::GenAttachments(int f_Width, int f_Height)
	{
		Clear();

		p_Width = f_Width;
		p_Height = f_Height;
		GenAttachmentsInternal(f_Width, f_Height);
	}

	template<unsigned int N>
	inline void Framebuffer<N>::UpdateSandboxs(int f_Sandboxs)
	{
		Clear();

		p_Sandboxs = f_Sandboxs;
		GenAttachmentsInternal(p_Width, p_Height);
	}

	template<unsigned int N>
	inline void Framebuffer<N>::EnableMultiSampling(int f_Sandboxs)
	{
		Clear();

		p_IsMultiSandbox = true;
		p_Sandboxs = f_Sandboxs;
		GenAttachmentsInternal(p_Width, p_Height);
	}

	template<unsigned int N>
	inline void Framebuffer<N>::DisableMultiSampling()
	{
		Clear();

		p_IsMultiSandbox = false;
		GenAttachmentsInternal(p_Width, p_Height);
	}

	template<unsigned int N>
	void Framebuffer<N>::GenAttachmentsInternal(int f_Width, int f_Height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, p_ID);

		std::initializer_list<std::tuple<TextureParameter, TextureParameterValue>> textureConfig({ });

		glGenRenderbuffers(1, &p_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, p_RenderBuffer);

		if (p_IsMultiSandbox)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, p_Sandboxs, GL_DEPTH24_STENCIL8, f_Width, f_Height);

			for (int i = 0; i < N; i++)
			{
				p_Textures[i] = new Texture2DMultisample(textureConfig, f_Width, f_Height, p_Attachments[i], p_Sandboxs);
				glFramebufferTexture2D(GL_FRAMEBUFFER, p_ColorAttachments[i], GL_TEXTURE_2D_MULTISAMPLE, p_Textures[i]->GetID(), 0);
			}
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, f_Width, f_Height);

			for (int i = 0; i < N; i++)
			{
				p_Textures[i] = new Texture2D(textureConfig, f_Width, f_Height, p_Attachments[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, p_ColorAttachments[i], GL_TEXTURE_2D, p_Textures[i]->GetID(), 0);
			}
		}

		glDrawBuffers(N, p_ColorAttachments);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}