#include "Shaders/GridShader.h"

#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

#include <core/Logger.h>
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SHADER
{
	GridShader::GridShader(const std::shared_ptr<GLShaderProgram>& glShader)
		: m_glProgram(glShader)
	{
		DEBUG_PTR(m_glProgram);
	}

	std::shared_ptr<GLShaderProgram> GridShader::getGLShaderProgram() const
	{
		return m_glProgram;
	}

	void GridShader::bind()
	{
		if (!m_glProgram) m_glProgram->bind();
		else
			Logger::warn("BasicShader program is can't binding!");
	}

	void GridShader::setMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
	{
		m_glProgram->setUniform("uModel",		model		);
		m_glProgram->setUniform("uView",		view		);
		m_glProgram->setUniform("uProjection", projection  );
	}
}