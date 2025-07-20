#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/fwd.hpp>

namespace MATERIAL   { struct Material;     };
namespace GLgraphics { class  RenderData;   };
namespace LIGHTING   { class  Light;		};
namespace SCENE		 { class  SceneObject;  };

namespace SHADER
{
	// forward declaration
	class GLShaderProgram;

	enum class ShaderType
	{
		BASIC,
		LIGHT,
		GRID,
	};


	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual ShaderType getType() const = 0;
		virtual std::shared_ptr<GLShaderProgram> getGLShaderProgram() const = 0;
		virtual void bind() = 0;
		virtual void setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights) = 0;
		virtual void setMaterial(const std::shared_ptr<MATERIAL::Material>& mat) = 0;
		virtual void setMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) = 0;

		const void setRenderDataObject(const std::shared_ptr<GLgraphics::RenderData>& renderData) { m_renderData = renderData; };

		virtual void setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& lightObject) = 0;
		virtual std::shared_ptr<SCENE::SceneObject> getShaderInterface() const = 0;

	protected:
		std::shared_ptr<GLgraphics::RenderData> m_renderData;
	};
}