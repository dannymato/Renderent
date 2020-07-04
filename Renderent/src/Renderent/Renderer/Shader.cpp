#include "repch.h"
#include "Shader.h"

#include "Renderent/Renderer/Renderer.h"
#include "Renderent/Platform/OpenGL/OpenGLShader.h"

namespace Renderent {

	

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "None API is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

	}

	Shader* Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "None API is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		}

	}

}