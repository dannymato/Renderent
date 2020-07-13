#include "repch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Renderent {

	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, float* vertices) 
	{
		RE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		RE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count, uint32_t* indices)
		: m_Count(count)
	{
		RE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}