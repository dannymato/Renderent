#pragma once

#include "Renderent/Renderer/Buffer.h"
#include "Renderent/Renderer/VertexArray.h"

namespace Renderent {

	class OpenGLVertexArray : public VertexArray {

	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override{
			return m_VertexBuffers;
		};
		virtual const Ref<IndexBuffer> GetIndexBuffer() const override{
			return m_IndexBuffer;
		}

	private:
		uint32_t m_ArrayHandle;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	};

}