#include "repch.h"
#include "VertexArray.h"

#include "Renderent/Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderent/Renderer/Renderer.h"

namespace Renderent {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				RE_CORE_ASSERT(false, "None API is not supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}
		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}