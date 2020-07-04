#include "repch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Renderent/Platform/OpenGL/OpenGLTexture.h"

namespace Renderent {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: RE_CORE_ASSERT(false, "API::None not yet implemented");
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);

		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
