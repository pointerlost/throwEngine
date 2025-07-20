#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <Shaders/ShaderInterface.h>

// forward declarations
namespace MATERIAL { struct Material;     };
namespace LIGHTING { class  Light;		  };
namespace SCENE	   { class  SceneObject;  };

namespace SHADER
{
	// forward declarations
	class IShader;
	class GLShaderProgram;
	class ShaderManager;

	class LightShader : public IShader
	{
	public:
		LightShader(const std::shared_ptr<GLShaderProgram>& glShader);

		virtual ShaderType getType() const { return m_shaderType; };
		virtual std::shared_ptr<GLShaderProgram> getGLShaderProgram() const;
		void bind() override;
		void setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights);
		void setMaterial(const std::shared_ptr<MATERIAL::Material>& mat) override {};
		void setMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) override;
		
		void setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& sceneVisualObject);
		std::shared_ptr<SCENE::SceneObject> getShaderInterface() const override;

	private:
		// This is the SceneObject that will be used for rendering the light
		std::shared_ptr<SCENE::SceneObject> m_lightShaderInterface = nullptr;
		// This is the OpenGL shader program that will be used for rendering
		std::shared_ptr<GLShaderProgram> m_glProgram = nullptr;
		// This is the type of the shader, used for identification
		ShaderType m_shaderType = ShaderType::LIGHT;
	};
}
