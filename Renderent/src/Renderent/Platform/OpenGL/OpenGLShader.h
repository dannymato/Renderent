#pragma once

#include "Renderent/Renderer/Shader.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Renderent {

	class OpenGLShader : public Shader {

	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		
		virtual ~OpenGLShader();

		virtual void Bind() const override;

		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadUniformInt(const int& value, const std::string& name);

		void UploadUniformMat3(const glm::mat3& matrix, const std::string& name);
		void UploadUniformMat4(const glm::mat4& matrix, const std::string& name);

		void UploadUniformFloat(const float& value, const std::string& name);
		void UploadUniformFloat2(const glm::vec2& value, const std::string& name);
		void UploadUniformFloat3(const glm::vec3& value, const std::string& name);
		void UploadUniformFloat4(const glm::vec4& value, const std::string& name);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map <GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string> shaderSource);
		

	private:
		uint32_t m_ProgramRef;
		std::string m_Name;
	};

}