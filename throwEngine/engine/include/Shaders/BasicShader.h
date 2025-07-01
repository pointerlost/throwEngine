#pragma once
#include "Shaders/ShaderInterface.h"

namespace LIGHTING { class Light;	  };
namespace MATERIAL { struct Material; };

namespace SHADER
{
	class GLShaderProgram;

	class BasicShader : public IShader
	{
	public:
		explicit BasicShader(const std::shared_ptr<GLShaderProgram>& glShader);

		virtual ShaderType getType() const { return m_type; };
		virtual std::shared_ptr<GLShaderProgram> getGLShaderProgram() const;
		void bind() override;
		void setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights) override;
		void setMaterial(const std::shared_ptr<MATERIAL::Material>& mat) override;
		void setMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) override;

	private:
		std::shared_ptr<GLShaderProgram> m_glProgram;
		glm::vec3 m_fallbackColor = glm::vec3(1.0, 0.0, 1.0f); // purple = error color

		ShaderType m_type = ShaderType::BASIC;
	};
}