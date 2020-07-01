#include "repch.h"
#include "Shader.h"

#include "Renderent/Renderer/Renderer.h"
#include "Renderent/Platform/OpenGL/OpenGLShader.h"

namespace Renderent {

	

	ShaderProgram* ShaderProgram::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "None API is not supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShaderProgram(vertexSrc, fragmentSrc);
		}

	}

}