#include "repch.h"
#include "OpenGLShader.h"

#include <filesystem>
#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Renderent {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		RE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		RE_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		Compile(shaderSources);

		std::filesystem::path path = filepath;
		m_Name = path.stem().string();

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc,
		const std::string& fragmentSrc) : m_Name(name) {
		RE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(sources);

	}

	void OpenGLShader::Bind() const{
		glUseProgram(m_ProgramRef);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::SetMat4(const glm::mat4& matrix, const std::string& name)
	{
		RE_PROFILE_FUNCTION();
		UploadUniformMat4(matrix, name);
	}

	void OpenGLShader::SetMat3(const glm::mat3& matrix, const std::string& name)
	{
		RE_PROFILE_FUNCTION();
		UploadUniformMat3(matrix, name);
	}

	void OpenGLShader::SetFloat4(const glm::vec4& value, const std::string& name) 
	{
		RE_PROFILE_FUNCTION();
		UploadUniformFloat4(value, name);
	}

	void OpenGLShader::SetFloat3(const glm::vec3& value, const std::string& name) 
	{
		RE_PROFILE_FUNCTION();
		UploadUniformFloat3(value, name);
	}

	void OpenGLShader::SetFloat2(const glm::vec2& value, const std::string& name) 
	{
		RE_PROFILE_FUNCTION();
		UploadUniformFloat2(value, name);
	}

	void OpenGLShader::SetFloat(const float& value, const std::string& name) 
	{
		RE_PROFILE_FUNCTION();
		UploadUniformFloat(value, name);
	}

	void OpenGLShader::SetInt(const int& value, const std::string& name)
	{
		RE_PROFILE_FUNCTION();
		UploadUniformInt(value, name);
	}

	void OpenGLShader::SetIntArray(int* values, uint32_t count, const std::string& name)
	{
		RE_PROFILE_FUNCTION();
		UploadUniformIntArray(values, count, name);
	}

	void OpenGLShader::UploadUniformInt(const int& value, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(int* values, uint32_t count, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformMat3(const glm::mat3& matrix, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const glm::mat4& matrix, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformFloat(const float& value, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const glm::vec2& value, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const glm::vec3& value, const std::string& name)
	{
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const glm::vec4& values, const std::string& name) {
		GLint location = glGetUniformLocation(m_ProgramRef, name.c_str());
		RE_CORE_ASSERT(location != -1, "Uniform location not found in shader");
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in) {

			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();

		}
		else {
			RE_CORE_ERROR("Could not load shaderfile: '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		RE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			RE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = 
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSource)
	{
		RE_PROFILE_FUNCTION();

		GLint program = glCreateProgram();
		RE_CORE_ASSERT(shaderSource.size() <= 2, "Too many shaders only 2 shaders supported");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSource) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				RE_CORE_ERROR("{0}", infoLog.data());
				RE_CORE_ASSERT(false, "Shader compilation failure!");
				// In this simple program, we'll just leave
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_ProgramRef = program;


		// Link our program
		glLinkProgram(m_ProgramRef);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_ProgramRef, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramRef, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramRef, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_ProgramRef);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Shader linking failure");
			// In this simple program, we'll just leave
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(m_ProgramRef, id);


	}

	OpenGLShader::~OpenGLShader() {
		RE_PROFILE_FUNCTION();
		glDeleteProgram(m_ProgramRef);
	}
}