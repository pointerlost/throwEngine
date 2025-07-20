#pragma once
#include <memory>
#include <glm/ext.hpp>
#include <iostream>

#include "Shaders/ShaderManager.h"
#include "graphics/Grid/GridRenderer.h"

// Forward Declarations;
namespace CAMERA   { class Camera;			};
namespace TEXTURE  { class Textures;		};
namespace MATERIAL { class MaterialLibrary; };
namespace SHADER   { class GLShaderProgram; };
namespace LIGHTING
{
	class Light;
	class LightManager;
};

namespace GLgraphics
{
	
	class RenderData
	{
	public:
		RenderData(std::shared_ptr<SHADER::ShaderManager> shaderManager,
			const std::shared_ptr<CAMERA::Camera>& camera,
			const std::shared_ptr<TEXTURE::Textures>& texture,
			const std::shared_ptr<MATERIAL::MaterialLibrary>& materialLib);

		~RenderData() = default;

		void setViewMatrix(const glm::mat4 view);
		void setProjectionMatrix(const glm::mat4 projection);

		void setLightManager(const std::shared_ptr<LIGHTING::LightManager>& lightManager) { m_lightManager = lightManager; };
		std::shared_ptr<LIGHTING::LightManager> getLightManager() { return m_lightManager; };

		void setGridRenderer(GRID::GridRenderer* gridRenderer) { m_gridRenderer = gridRenderer; };
		GRID::GridRenderer* getGridRenderer() { return m_gridRenderer; };
		
		glm::mat4 getViewMatrix() const { return view; };
		glm::mat4 getProjectionMatrix() const { return projection; };

		std::shared_ptr<SHADER::ShaderManager> getShaderManager() const { return m_shaderManager; };
		std::shared_ptr<SHADER::IShader> getShaderInterface(const std::string& name);
		std::shared_ptr<SHADER::GLShaderProgram>& getWrapperGLShader();
		//std::shared_ptr<SHADER::Shader> getWrappedShader(const std::string& name);
		const std::shared_ptr<CAMERA::Camera> getCamera() const { return camera; };
		const std::shared_ptr<TEXTURE::Textures> getTexture() const { return texture; };
		const std::shared_ptr<MATERIAL::MaterialLibrary>& getMaterial() const { return m_material; };

		glm::vec3 getGlobalAmbient() const { return g_ambientLight; };

		void update();

	protected:
		
		std::shared_ptr<LIGHTING::LightManager> m_lightManager;
		std::shared_ptr<SHADER::ShaderManager> m_shaderManager;
		std::shared_ptr<TEXTURE::Textures> texture;
		std::shared_ptr<MATERIAL::MaterialLibrary> m_material;
		std::shared_ptr<CAMERA::Camera> camera;
		GRID::GridRenderer* m_gridRenderer = nullptr;

	private:
		// Copy MVP matrices into RenderData
		// Because if we don't copy, we have to give reference all of the time, so this is cause complexity
		glm::mat4 view;
		glm::mat4 projection;

		glm::vec3 g_ambientLight;
	};
}
