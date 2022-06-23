#include "gfxpch.h"
#include "Buffer.h"

namespace Jam
{

	VertexBuffer::VertexBuffer()
		: p_ID(0)
	{
	}

	void VertexBuffer::Init()
	{
		glCreateBuffers(1, &p_ID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &p_ID);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_ID);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* f_Data, uint32_t f_Size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, p_ID);
		glBufferData(GL_ARRAY_BUFFER, f_Size, f_Data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer()
		: p_ID(0), p_Count(0)
	{
	}

	void IndexBuffer::Init()
	{
		glCreateBuffers(1, &p_ID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &p_ID);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ID);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(uint32_t* f_Indices, uint32_t f_Count)
	{
		p_Count = f_Count;
		glBindBuffer(GL_ARRAY_BUFFER, p_ID);
		glBufferData(GL_ARRAY_BUFFER, f_Count * sizeof(unsigned int), f_Indices, GL_STATIC_DRAW);
	}

	VertexArray::VertexArray()
		: p_ID(0)
	{
	}

	void VertexArray::Init()
	{
		glCreateVertexArrays(1, &p_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &p_ID);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(p_ID);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	static GLenum ShaderElementType(ShaderDataType f_Element)
	{
		switch (f_Element)
		{
		case ShaderDataType::Float:   return GL_FLOAT;
		case ShaderDataType::Float3:  return GL_FLOAT;
		case ShaderDataType::Float2:  return GL_FLOAT;
		case ShaderDataType::Float4:  return GL_FLOAT;
		case ShaderDataType::Int:     return GL_INT;
		}
		return 0;
	}

	static uint32_t ShaderDataTypeSize(ShaderDataType f_Type)
	{
		switch (f_Type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Int:      return 4;
		}
		return 0;
	}

	static uint32_t ShaderDataTypeCount(ShaderDataType f_Type)
	{
		switch (f_Type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Int:     return 1;
		}
		return 0;
	}

	void VertexArray::BindVertexBuffer(VertexBuffer* f_VertexBuffer, BufferLayout& f_Layout)
	{
		glBindVertexArray(p_ID);
		f_VertexBuffer->Bind();

		int a = 0;
		int offset = 0;
		for (ShaderDataType e : f_Layout.GetElements())
		{
			glVertexAttribPointer(a, ShaderDataTypeCount(e), ShaderElementType(e), GL_FALSE, f_Layout.GetStride(), (void*)offset);
			glEnableVertexAttribArray(a);
			a++;
			offset += ShaderDataTypeSize(e);
		}
	}

	void VertexArray::BindIndexBuffer(IndexBuffer* f_IndexBuffer)
	{
		glBindVertexArray(p_ID);
		f_IndexBuffer->Bind();
	}

	BufferLayout::BufferLayout(std::initializer_list<ShaderDataType> f_Elements)
		: p_Stride(0)
	{
		p_Elements = f_Elements;
		p_Stride = 0;
		for (ShaderDataType e : p_Elements)
		{
			p_Stride += ShaderDataTypeSize(e);
		}
	}

}
