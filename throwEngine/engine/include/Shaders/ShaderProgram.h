#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

namespace SHADER {

	class GLShaderProgram
	{
	public:
		GLShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~GLShaderProgram();
		
		bool SetUpShader(const std::string& vertexPath, const std::string& fragmentPath);

		bool bind() const;
		void unbind() const;

		void cleanUp() const;

		uint32_t getProgramID() const;

		bool hasUniform(const std::string& name) const;
		void setUniform(const std::string& name, const glm::mat4 type) const;
		void setUniforms(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection) const;
		void setUniforms(const glm::mat4 mvp) const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, const glm::vec2& value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, const glm::vec4& value) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setMat2(const std::string& name, const glm::mat2& mat) const;
		void setMat3(const std::string& name, const glm::mat3& mat) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;

	protected:
		void createShader(GLenum shaderType, const char* shaderPath);
		bool createProgram(unsigned int vertex, unsigned int fragment);
		bool checkShaderCompilingErrors(GLenum shaderType, unsigned int shader);

	private:
		unsigned int m_vertexID, m_fragmentID;
		unsigned int m_programID;

		bool m_isValid;
	};
}


