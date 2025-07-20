#include "Scene/Scene.h"

#include <Scene/SceneObject.h>

#include <graphics/Mesh/GLMeshFactory.h>

#include "graphics/Mesh/GLMeshCube3D.h"

#include "graphics/Mesh/GLMeshSphere3D.h"

#include "graphics/Mesh/GLMeshCircle3D.h"

#include "graphics/Mesh/GLMeshSquare3D.h"

#include "graphics/Mesh/GLMeshTriangle3D.h"

#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

#include "Scene/SceneObjectFactory.h"

#include "graphics/Grid/GridSystem.h"

#include <graphics/GLTransformations/Transformations.h>

#include "graphics/Lighting/LightManager.h"

#include <graphics/Lighting/Light.h>

#include "shaders/ShaderManager.h"

#include "shaders/BasicShader.h"

#include "shaders/LightShader.h"

#include "shaders/GridShader.h"

#include "graphics/Material/MaterialLib.h"

#include "Input/InputComponent.h"

#include "graphics/Camera/Camera.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

constexpr const char* BASICSHADER_NAME = "basicShader";
constexpr const char* LIGHTSHADER_NAME = "lightObjectShader";

namespace SCENE
{
	Scene::Scene(std::shared_ptr<GLgraphics::MeshData3D> data3D, Input::InputContext context)
		:meshData3D(data3D), inputContext(context)
	{
		DEBUG_PTR(meshData3D);
	}

	void Scene::debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		//DEBUG MODE
		for (auto& obj : sceneObjects) {
			try {
				if (obj) {
					obj->draw(view, projection, renderData);
				}
				else {
					throw std::runtime_error("trying to draw an object that returns nullptr!");
					// using throw can be costly (especially in per-frame loops)
				}
			}
			catch (const std::exception& e) {
				Logger::error("Error drawing object " + obj->getObjectName() + ": " + e.what());
				continue;
			}
		}
	}

	bool Scene::SetUpResources(const std::shared_ptr<MATERIAL::MaterialLibrary>& library)
    {
		if (!m_sceneObjectFactory) {
			Logger::error("Scene can't setup m_sceneObjectFactory is nullptr!");
			return false;
		}

        auto basicShader = m_shaderManager->getShaderInterface(BASICSHADER_NAME);
        if (!basicShader) {
            Logger::warn("[Warning] basicShader not found, using fallback");
            basicShader = m_shaderManager->getShaderInterface("default");
            if (!basicShader) {
                Logger::error("[Error] Fallback shader not found!");
                return false;
            }
        }

        auto lightShader = m_shaderManager->getShaderInterface(LIGHTSHADER_NAME);
        if (!lightShader) {
            Logger::warn("[Warning] lightObjectShader not found, using fallback");
            lightShader = m_shaderManager->getShaderInterface("default");
            if (!lightShader) {
                Logger::error("[Error] Fallback shader not found!");
                return false;
            }
        }

		std::shared_ptr<LIGHTING::Light> lightObj;
		std::shared_ptr<SceneObject> lightObject;

		std::tie(lightObj, lightObject) = m_sceneObjectFactory->createSpotLight(
			"sun", // name of the light
			glm::vec3{ 30.0f, 25.0f, 15.0f }, // position of the light
			lightShader, // shader for the light
			library // material library for the light
		);
		getLightManager()->addLightToVec(lightObj);
		AddObjectIntoScene(lightObject);

		std::tie(lightObj, lightObject) = m_sceneObjectFactory->createPointLight(
			"sun1",
			glm::vec3{ 30.0f, 25.0f, 15.0f },
			lightShader,
			library
		);
		getLightManager()->addLightToVec(lightObj);
		AddObjectIntoScene(lightObject);

		auto cube1 = m_sceneObjectFactory->createCube(
			"silver",
			glm::vec3{ 0.0f, 0.0f, 0.0f },
			basicShader
		);
		AddObjectIntoScene(cube1);

		// Floor
        const auto floor = m_sceneObjectFactory->createCube(
			"gold",
            glm::vec3{ 0.0f, -2.5f, 0.0f },
            basicShader
        );
        floor->getTransform()->setScale(glm::vec3{ 50.5f, 1.5f, 50.5f });
        AddObjectIntoScene(floor);

		// create your own object-specific materials
		giveYourOwnMaterialsToObjects(library);

        Logger::info("[Scene] SetUpResources successful!");
        return true;
    }

	void Scene::giveYourOwnMaterialsToObjects(const std::shared_ptr<MATERIAL::MaterialLibrary>& library)
	{
		// copy materials to the sceneobjects
		for (auto& obj : sceneObjects) {
			if (obj->getMaterialInstance()) {
				Logger::warn("[Scene::giveYourOwnMaterialsToObjects] '" + obj->getObjectName() + "' already has a material instance, skipping!");
				continue;
			}
			else {
				obj->initializeMaterial(library);
			}
		}
	}

	bool Scene::initGrid(std::shared_ptr<GLgraphics::RenderData> renderData)
	{
		auto gridShader = m_shaderManager->getShaderInterface("gridShader");
		if (!gridShader) {
			Logger::warn("[Warning] lightObjectShader not found, using fallback");
			gridShader = m_shaderManager->getShaderInterface("default");
			if (!gridShader) {
				Logger::error("[Error] Fallback shader not found!");
				return false;
			}
		}
		
		if (!m_gridSystem) {
			Logger::warn("[Scene::initGrid] m_gridSystem returning nullptr");
			return false;
		}
		
		// this function receives messages from the "Logger", no need to retrieve them again
		if (!m_gridSystem->getRenderer()->setGridShaderInterface(gridShader))
			return false;

		Logger::info("[Scene::initGrid] successfull!");
		return true;
	}

	void Scene::drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		if (auto gridRenderer = renderData->getGridRenderer()) {
			if (auto shader = gridRenderer->getGridShader()) {
				auto shaderProgram = shader->getGLShaderProgram();
				if (shaderProgram) {
					shaderProgram->bind();
					shaderProgram->setMat4("u_view", view);
					shaderProgram->setMat4("u_projection", projection);
					gridRenderer->draw();
				}
			}
		}
	}

	void Scene::drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		drawGrid(view, projection, renderData);
	
		for (auto& obj : sceneObjects)
		{
			if (!obj) {
				Logger::error("Object '" + obj->getObjectName() + "' is nullptr, skipping draw!");
				Logger::warn("trying to draw an object that returns nullptr!");
				return;
			}
			else if (obj->getObjectType() == ObjectType::Visual)
				obj->draw(view, projection, renderData);
			else if (obj->getObjectType() == ObjectType::LightVisual)
				obj->drawLight(view, projection, renderData);
			else
				Logger::warn("unknown object type, skipping draw!");
		}
	}

	void Scene::createLightObjects(const std::shared_ptr<MATERIAL::MaterialLibrary>& library)
	{
	}

	void Scene::AddObjectIntoScene(const std::shared_ptr<SceneObject>& object)
	{
		//std::cout << "object name is: " << object->getObjectName() << "\n";
		std::string objectName = object->getObjectName();
		int count = 0;

		while (checkObjectIsAvailable(objectName)) {
			objectName += std::to_string(count++);
		}

		object->setObjectName(objectName);

		sceneObjectsMap[objectName] = object;

		sceneObjects.push_back(object);
	}

	bool Scene::checkObjectIsAvailable(const std::string& name)
	{
		auto it = sceneObjectsMap.find(name);

		if (it != sceneObjectsMap.end()) {
			return true;
		}

		return false;
	}

	void Scene::AddSceneObjectIntoMap(const std::string& name, const std::shared_ptr<SceneObject> object)
	{
		if (!checkObjectIsAvailable(name)) {
			Logger::warn("this object already exist, you can't add!");
			return;
		}
		sceneObjectsMap[name] = object;
	}

	std::shared_ptr<SceneObject> Scene::getSpesificSceneObjectWithName(const std::string& name)
	{
		auto it = sceneObjectsMap.find(name);

		return ( it != sceneObjectsMap.end() ) ? it->second : nullptr;
	}

	uint32_t Scene::uniqueObjectIDGenerator()
	{
		return m_uniqueID++;
	}

	void Scene::addInputComponent(std::shared_ptr<Input::IInputComponent> component)
	{
		m_inputComponents.push_back(component);
		Logger::info("[SceneObject] Input Component added. Total: " + std::to_string(m_inputComponents.size()));
	}

	void Scene::updateInputComponents()
	{
		for (auto& component : m_inputComponents) {
			component->processInput(*this);
		}
	}

}