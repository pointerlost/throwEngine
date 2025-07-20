#include "Scene/SceneObject.h"

#include "graphics/Renderer/RenderData.h"

#include "Scene/Scene.h"

#include "Input/InputComponent.h"

#include "graphics/GLTransformations/Transformations.h"

#include "Shaders/ShaderProgram.h"
#include "Shaders/LightShader.h"
#include "Shaders/BasicShader.h"
#include "Shaders/GridShader.h"

#include "graphics/Mesh/GLMeshTriangle3D.h"
#include "graphics/Mesh/GLMeshSquare3D.h"
#include "graphics/Mesh/GLMeshCube3D.h"
#include "graphics/Mesh/GLMeshCircle3D.h"

#include <graphics/Textures/Textures.h>

#include "graphics/Material/MaterialLib.h"

#include "graphics/Lighting/LightManager.h"

#include "graphics/Camera/Camera.h"

#include <core/Logger.h>
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SCENE
{
	SceneObject::SceneObject(const std::shared_ptr<GLgraphics::IMesh> mesh, const std::string& name,
		const std::string& materialName)
		:
		m_mesh(mesh), m_transform(std::make_shared<GLgraphics::Transformations>()),
		m_objectName(name), m_materialName(materialName)
	{
		DEBUG_PTR(m_transform);
		DEBUG_PTR(m_mesh);
	}

	bool SceneObject::validateRenderState(const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		// shader is an interface
		auto& iShader = m_shaderInterface;
		DEBUG_PTR(iShader);

		if (!iShader) {
			Logger::warn("[WARN] [SceneObject] Shader Interface not found! Skipping draw.");
			return false;
		}

		auto lightManager = renderData->getLightManager();
		DEBUG_PTR(lightManager);

		if (!lightManager) {
			Logger::warn("[WARN] [SceneObject] LightManager missing! Skipping draw.");
			return false;
		}

		auto& material = m_materialInstance;
		DEBUG_PTR(material);

		if (!material) {
			Logger::info("[SceneObject] Material missing! Not skipping but assign default material!");
			material = renderData->getMaterial()->getDefaultMaterial();
			return false;
		}

		auto texture = renderData->getTexture();
		DEBUG_PTR(texture);

		if (!texture) {
			Logger::warn("[WARN] [SceneObject] Texture missing! Skipping draw.");
			return false;
		}

		auto camera = renderData->getCamera();
		DEBUG_PTR(camera);

		if (!camera) {
			Logger::warn("[WARN] [SceneObject] Camera missing! Skipping draw.");
			return false;
		}

		return true;
	}

	bool SceneObject::validateRenderStateLight(const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		auto& lightShader = m_shaderInterface;
		if (!lightShader) {
			Logger::error("[ERROR] [SceneObject::drawLight] LightShader is nullptr!");
			return false;
		}

		// shader is an interface
		auto& iShader = m_shaderInterface;
		DEBUG_PTR(iShader);

		if (!iShader) {
			Logger::warn("[WARN] [SceneObject] Shader Interface not found! Skipping draw.");
			return false;
		}

		auto lightManager = renderData->getLightManager();
		DEBUG_PTR(lightManager);

		if (!lightManager) {
			Logger::warn("[WARN] [SceneObject] LightManager missing! Skipping draw.");
			return false;
		}

		auto camera = renderData->getCamera();
		DEBUG_PTR(camera);

		if (!camera) {
			Logger::warn("[WARN] [SceneObject] Camera missing! Skipping draw.");
			return false;
		}

		return true;
	}

	void SceneObject::initializeMaterial(const std::shared_ptr<MATERIAL::MaterialLibrary>& library)
	{
		auto base = library->getMaterialByName(m_materialName);
		m_materialInstance = std::make_shared<MATERIAL::Material>(*base);
	}

	void SceneObject::setMaterialInstance(std::shared_ptr<MATERIAL::Material> instance)
	{
		m_materialInstance = instance;
	}

	void SceneObject::setShaderInterface(const std::shared_ptr<SHADER::IShader> shader)
	{
		if (!shader) {
			Logger::error("[ERROR] [SceneObject::setShaderInterface] Shader is nullptr!");
			return;
		}
		if (shader->getType() == SHADER::ShaderType::LIGHT) {
			m_shaderInterface = std::dynamic_pointer_cast<SHADER::LightShader>(shader);
			if (!m_shaderInterface) {
				Logger::error("[ERROR] [SceneObject::setShaderInterface] Failed to cast to LightShader!");
			}
		}
		else if (shader->getType() == SHADER::ShaderType::BASIC) {
			m_shaderInterface = std::dynamic_pointer_cast<SHADER::BasicShader>(shader);
			if (!m_shaderInterface) {
				Logger::error("[ERROR] [SceneObject::setShaderInterface] Failed to cast to BasicShader!");
			}
		}
		else {
			Logger::warn("[WARN] [SceneObject::setShaderInterface] Unknown shader type, using default shader.");
		}
	}

	void SceneObject::draw(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		// check nullptr and other debugging stuff
		if (!validateRenderState(renderData)) {
			Logger::error("[ERROR] [SceneObject::draw] validateRenderState pointer initialization failed!");
			return;
		}

		const glm::mat4 model = m_transform->getModelMatrix();

		auto& iShader = m_shaderInterface;
		iShader->setRenderDataObject(renderData);
		auto lightManager = renderData->getLightManager();
		auto &material = m_materialInstance;
		auto texture = renderData->getTexture();
		auto camera = renderData->getCamera();

		iShader->bind();

		if (iShader->getType() != SHADER::ShaderType::LIGHT)
			lightManager->uploadLights(iShader->getGLShaderProgram());

		texture->bind(material->m_diffuseTextureID, material->m_specularTextureID);

		iShader->setMaterial(material);
		iShader->setLights(lightManager->getLightsByVec());
		iShader->setMatrices(model, view, projection, camera->getCameraPosition());

		// draw per object
		m_mesh->drawMeshObject();
	}

	void SceneObject::drawLight(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		if (!validateRenderStateLight(renderData)) {
			Logger::error("[ERROR] [SceneObject::drawLight] validateRenderStateLight pointer initialization failed!");
			return;
		}

		auto& lightShader = m_shaderInterface;
		
		lightShader->bind();

		const glm::mat4 model = m_transform->getModelMatrix();
		glm::vec3 cameraPos = renderData->getCamera()->getCameraPosition();

		lightShader->setMatrices(model, view, projection, cameraPos);
		lightShader->setLights(renderData->getLightManager()->getLightsByVec());

		m_mesh->drawMeshObject();
	}

}