#include "repch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Renderent {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		}
		RE_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		RE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_ArrayHandle);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_ArrayHandle);
	}

	void OpenGLVertexArray::Bind() const
	{
		RE_PROFILE_FUNCTION();

		glBindVertexArray(m_ArrayHandle);
	}

	void OpenGLVertexArray::Unbind() const
	{
		RE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		RE_PROFILE_FUNCTION();

		RE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_ArrayHandle);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				vertexBuffer->GetLayout().GetStride(),
				(void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		RE_PROFILE_FUNCTION();

		glBindVertexArray(m_ArrayHandle);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}


}


