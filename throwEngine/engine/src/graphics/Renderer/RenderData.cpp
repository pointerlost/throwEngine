#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

#include "Shaders/ShaderManager.h"

#include "graphics/Camera/Camera.h"

#include <graphics/Textures/Textures.h>

#include "graphics/Material/MaterialLib.h"

#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace GLgraphics
{
	RenderData::RenderData(std::shared_ptr<SHADER::ShaderManager> shaderManager, const std::shared_ptr<CAMERA::Camera>& camera,
		const std::shared_ptr<TEXTURE::Textures>& texture, const std::shared_ptr<MATERIAL::MaterialLibrary>& materialLib)
		: g_ambientLight(glm::vec3(0.1, 0.1, 0.1)), m_shaderManager(shaderManager),
		camera(camera), texture(texture), m_material(materialLib)
	{
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);

		DEBUG_PTR(shaderManager);
		DEBUG_PTR(camera);
		DEBUG_PTR(texture);
		DEBUG_PTR(m_material);
	}
	
	// VIEW (CAMERA) STUFF
	void RenderData::setViewMatrix(const glm::mat4 view)
	{
		this->view = view;
	}

	// PROJECTION STUFF
	void RenderData::setProjectionMatrix(const glm::mat4 projection)
	{
		this->projection = projection;
	}

	std::shared_ptr<SHADER::IShader> RenderData::getShaderInterface(const std::string& name)
	{
		return m_shaderManager->getShaderInterface(name);
	}

	std::shared_ptr<SHADER::GLShaderProgram>& RenderData::getWrapperGLShader()
	{
		return m_shaderManager->getWrapperGLShader();
	}

	// UPDATE MVP
	void RenderData::update()
	{
		view = camera->getViewMatrix();
		projection = camera->getProjectionMatrix();
	}

}