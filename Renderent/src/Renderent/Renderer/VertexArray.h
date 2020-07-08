#pragma once

#include "Renderent/Renderer/Buffer.h"

namespace Renderent {

	class VertexArray {
	public:
		static VertexArray* Create();

		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

	};

}