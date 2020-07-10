#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Renderent {

	class Shader {
	public:
		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~Shader() = default;

		virtual const std::string& GetName() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetMat4(const glm::mat4& matrix, const std::string& name) = 0;
		virtual void SetMat3(const glm::mat3& matrix, const std::string& name) = 0;
		virtual void SetFloat4(const glm::vec4& value, const std::string& name) = 0;
		virtual void SetFloat3(const glm::vec3& value, const std::string& name) = 0;
		virtual void SetFloat2(const glm::vec2& value, const std::string& name) = 0;
		virtual void SetFloat(const float& value, const std::string& name) = 0;
		virtual void SetInt(const int& value, const std::string& name) = 0;

	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}