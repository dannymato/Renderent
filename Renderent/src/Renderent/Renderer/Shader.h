#pragma once

#include <string>

namespace Renderent {

	class ShaderProgram {
	public:
		static ShaderProgram* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~ShaderProgram() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	};

}