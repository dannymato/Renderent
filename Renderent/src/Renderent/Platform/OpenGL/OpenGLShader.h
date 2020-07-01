#pragma once

#include "Renderent/Renderer/Shader.h"

namespace Renderent {

	class OpenGLShaderProgram : public ShaderProgram {

	public:
		OpenGLShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShaderProgram();

		virtual void Bind() const override;

		virtual void Unbind() const override;

	private:
		uint32_t m_ProgramRef;

	};

}