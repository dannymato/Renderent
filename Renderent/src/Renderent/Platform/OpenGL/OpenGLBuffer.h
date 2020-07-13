#pragma once

#include "Renderent/Renderer/Buffer.h"

namespace Renderent {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:

		OpenGLVertexBuffer(size_t size, float* vertices);
		OpenGLVertexBuffer(size_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_BufferLayout; }
		virtual void SetLayout(const BufferLayout& layout) override {
			m_BufferLayout = layout;
		}

		virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {

	public:

		OpenGLIndexBuffer(uint32_t count, uint32_t* indices);

		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}