#pragma once

#include "Core/Core.h"
#include <string>
#include <glm/glm.hpp>
#include <filesystem>

typedef unsigned int GLenum;

namespace Jah {

	class Shader
	{
	public:
		Shader(const std::filesystem::path& filepath);
		Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		const std::string& GetName() const { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::filesystem::path& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
	};

	class ShaderLibrary
	{
	public:
		void Add(const Shared<Shader>& shader);
		void Add(const std::string& name, const Shared<Shader>& shader);
		Shared<Shader> Load(const std::filesystem::path& filepath);
		Shared<Shader> Load(const std::string& name, std::filesystem::path& filepath);

		Shared<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Shared<Shader>> m_Shaders;
	};

}