#include "repch.h"

#include "Framebuffer.h"

#include "Renderent/Renderer/Renderer.h"
#include "Renderent/Renderer/RendererAPI.h"
#include "Renderent/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Renderent {
	Ref<Framebuffer> Renderent::Framebuffer::Create(const FramebufferProps& props)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "Renderer API None is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(props);
		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
