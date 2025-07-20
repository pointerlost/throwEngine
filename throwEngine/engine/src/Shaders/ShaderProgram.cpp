#include "Shaders/ShaderProgram.h"

#include "core/Logger.h"

namespace SHADER
{
	GLShaderProgram::GLShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
	{
		/* do it some stuff */
		m_vertexID = 0;
		m_fragmentID = 0;
		m_programID = 0;
		m_isValid = false;

		try {
			m_isValid = SetUpShader(vertexPath, fragmentPath);
		}
		catch (...) {
			cleanUp();
			m_isValid = false;
		}

		if (!m_isValid) {
			Logger::error("Shader program creation failed - using fallback shader");
		}
	}

	GLShaderProgram::~GLShaderProgram()
	{
		cleanUp();
	}

	bool GLShaderProgram::SetUpShader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		createShader(GL_VERTEX_SHADER, vertexSource.c_str());
		createShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
		bool linked = createProgram(m_vertexID, m_fragmentID);
		return linked;
	}

	bool GLShaderProgram::bind() const
	{
		if (!m_isValid) {
			Logger::warn("[GLShaderProgram] shader m_isvalid returning false!");
			return false;
		}
		glUseProgram(m_programID);
		return true;
	}

	void GLShaderProgram::unbind() const
	{
		glUseProgram(0);
	}

	bool GLShaderProgram::hasUniform(const std::string& name) const {
		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

		if (currentProgram != m_programID) {
			Logger::error("Shader not bound when checking uniform: " + name +
				" | expected: " + std::to_string(m_programID) +
				" | current: " + std::to_string(currentProgram));
			return false;
		}

		return glGetUniformLocation(m_programID, name.c_str()) != -1;
	}

	void GLShaderProgram::cleanUp() const
	{
		glDeleteProgram(m_programID);
		glDeleteShader(m_vertexID);
		glDeleteShader(m_fragmentID);
	}

	void GLShaderProgram::createShader(GLenum shaderType, const char* shaderSourceCode)
	{
		if (shaderType == GL_VERTEX_SHADER) {
			m_vertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(m_vertexID, 1, &shaderSourceCode, NULL);
			glCompileShader(m_vertexID);
			checkShaderCompilingErrors(shaderType, m_vertexID);
		}
		else if (shaderType == GL_FRAGMENT_SHADER) {
			m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(m_fragmentID, 1, &shaderSourceCode, NULL);
			glCompileShader(m_fragmentID);
			checkShaderCompilingErrors(shaderType, m_fragmentID);
		}
	}

	bool GLShaderProgram::createProgram(unsigned int vertex, unsigned int fragment)
	{
		m_programID = glCreateProgram();
		glAttachShader(m_programID, vertex);
		glAttachShader(m_programID, fragment);
		glLinkProgram(m_programID);

		return checkShaderCompilingErrors(NULL, m_programID);
	}

	bool GLShaderProgram::checkShaderCompilingErrors(GLenum shaderType, unsigned int objectID)
	{
		int success = 0;
		char infoLog[1024];

		if (shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER)
		{
			glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(objectID, 1024, NULL, infoLog);
				Logger::error("[GLShaderProgram] Shader compilation error (" + std::to_string(shaderType) + "): " + std::string(infoLog));
				return false;
			}
		}
		else // shaderType is not vertex or fragment shader, assume program object
		{
			glGetProgramiv(objectID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(objectID, 1024, NULL, infoLog);
				Logger::error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
				return false;
			}
		}
		return true;
	}

	uint32_t GLShaderProgram::getProgramID() const
	{
		return m_programID;
	}

	void GLShaderProgram::setUniform(const std::string& name, const glm::mat4 type) const
	{
		setMat4(name, type);
	}

	void GLShaderProgram::setUniforms(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection) const
	{
		glm::mat4 mvp = projection * view * model;
		setMat4("MVP", mvp);
	}

	void GLShaderProgram::setUniforms(const glm::mat4 mvp) const
	{
		setMat4("MVP", mvp);
	}

	// -------------------------- SET BOOL ----------------------------------
	void GLShaderProgram::setBool(const std::string& name, bool value) const
	{
		GLint loc = glGetUniformLocation(m_programID, name.c_str());
		if (loc != -1)
			glUniform1i(loc, (int)value);
		else
			Logger::warn("[Warning] uniform '" + name + "' not found or optimized out!");
	}

	// -------------------------- SET INT ----------------------------------
	void GLShaderProgram::setInt(const std::string& name, int value) const
	{
		GLint loc = glGetUniformLocation(m_programID, name.c_str());
		if (loc != -1)
			glUniform1i(loc, value);
		else
			Logger::warn("[Warning] uniform '" + name + "' not found or optimized out!");
	}

	// -------------------------- SET FLOAT ----------------------------------
	void GLShaderProgram::setFloat(const std::string& name, float value) const
	{
		GLint loc = glGetUniformLocation(m_programID, name.c_str());
		if (loc != -1)
			glUniform1f(loc, value);
		else
			Logger::warn("[Warning] uniform '" + name + "' not found or optimized out!");
	}
	// -------------------------- SET VEC ----------------------------------
	void GLShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
	}

	void GLShaderProgram::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
	}

	void GLShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
	{
		GLint loc = glGetUniformLocation(m_programID, name.c_str());
		if (loc != -1)
			glUniform3fv(loc, 1, &value[0]);
		else
			Logger::warn("[Warning] uniform '" + name + "' not found or optimized out!");
	}

	void GLShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
	}
	
	void GLShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
	}
	void GLShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_programID, name.c_str()), x, y, z, w);
	}

	// -------------------------- SET MAT ----------------------------------
	void GLShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void GLShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void GLShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}