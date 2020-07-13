#include "repch.h"
#include "VertexArray.h"

#include "Renderent/Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderent/Renderer/Renderer.h"

namespace Renderent {

	Ref<VertexArray> VertexArray::Create()
	{
		RE_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				RE_CORE_ASSERT(false, "None API is not supported");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}
		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}