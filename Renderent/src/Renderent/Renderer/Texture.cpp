#include "repch.h"
#include "Texture.h"

#include "Renderent/Platform/OpenGL/OpenGLTexture.h"
#include "Renderent/Renderer/Renderer.h"

namespace Renderent {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: RE_CORE_ASSERT(false, "API::None not yet implemented"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);

		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: RE_CORE_ASSERT(false, "API::None not yet implemented"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI");
		return nullptr;
	}
}
