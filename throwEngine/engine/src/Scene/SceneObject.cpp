#include "Scene/SceneObject.h"

#include "graphics/Renderer/RenderData.h"

#include "Scene/Scene.h"

#include "Input/InputComponent.h"

#include "graphics/GLTransformations/Transformations.h"

#include "Shaders/ShaderProgram.h"
#include "Shaders/LightShader.h"
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

		const auto& wrapperGLShader = renderData->getWrapperGLShader();
		DEBUG_PTR(wrapperGLShader);

		if (!wrapperGLShader) {
			Logger::warn("[WARN] [SceneObject] Helper Shader missing! Skipping draw.");
			return false;
		}

		auto lightManager = renderData->getLightManager();
		DEBUG_PTR(lightManager);

		if (!lightManager) {
			Logger::warn("[WARN] [SceneObject] LightManager missing! Skipping draw.");
			return false;
		}

		auto material = renderData->getMaterial()->getMaterialByName(m_materialName);
		DEBUG_PTR(material);

		if (!material) {
			Logger::warn("[WARN] [SceneObject] Material missing! Skipping draw.");
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
		const auto& wrapperShader = renderData->getWrapperGLShader();
		auto lightManager = renderData->getLightManager();
		auto material = renderData->getMaterial()->getMaterialByName(m_materialName);
		auto texture = renderData->getTexture();
		auto camera = renderData->getCamera();

		iShader->bind();

		lightManager->uploadLights(iShader->getGLShaderProgram());

		iShader->setMaterial(material);
		iShader->setLights(lightManager->getLights());
		iShader->setMatrices(model, view, projection, camera->getCameraPosition());
		
		texture->bind();

		// draw per object
		m_mesh->drawMeshObject();
	}

	void SceneObject::update(SCENE::Scene &scene)
	{
		for (auto& component : inputComponents) {
			component->processInput(scene);
		}
	}

	void SceneObject::addInputComponent(std::shared_ptr<InputComponent::IInputComponent> component)
	{
		inputComponents.push_back(component);
		Logger::info("[SceneObject] Input Component added. Total: " + std::to_string(inputComponents.size()));
	}

}