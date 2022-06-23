#pragma once

namespace Jam
{

	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Int
	};

	class BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<ShaderDataType> f_Elements);

		~BufferLayout() = default;
		BufferLayout(const BufferLayout&) = default;

		std::vector<ShaderDataType>& GetElements() { return p_Elements; }
		uint32_t GetStride() { return p_Stride; }

	private:
		std::vector<ShaderDataType> p_Elements;
		uint32_t p_Stride;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer();

		void Init();

		~VertexBuffer();
		VertexBuffer(const VertexBuffer&) = default;

		void Bind();
		void Unbind();

		void SetData(const void* f_Data, uint32_t f_Size);

	private:
		unsigned int p_ID;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer();

		void Init();

		~IndexBuffer();
		IndexBuffer(const IndexBuffer&) = default;

		void Bind();
		void Unbind();

		void SetData(uint32_t* f_Indices, uint32_t f_Count);

		unsigned int GetCount() { return p_Count; }

	private:
		uint32_t p_Count;
		uint32_t p_ID;
	};

	class VertexArray
	{
	public:
		VertexArray();

		void Init();

		~VertexArray();
		VertexArray(const VertexArray&) = default;

		void Bind();
		void Unbind();

		void BindVertexBuffer(VertexBuffer* f_VertexBuffer, BufferLayout& f_Layout);
		void BindIndexBuffer(IndexBuffer* f_IndexBuffer);

	private:
		unsigned int p_ID;
	};

}