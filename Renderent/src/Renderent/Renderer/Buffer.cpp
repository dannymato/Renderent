#include "repch.h"
#include "Buffer.h"

#include "Renderent/Platform/OpenGL/OpenGLBuffer.h"
#include "Renderent/Renderer/Renderer.h"

namespace Renderent {

	VertexBuffer* VertexBuffer::Create(size_t size, float* vertices) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "Renderer API None is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(size, vertices);
		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t count, uint32_t* indices) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "Renderer API None is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(count, indices);
		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}