#include "Scene/Scene.h"

#include <Scene/SceneObject.h>

#include "Input/InputComponent.h"

#include <graphics/Mesh/GLMeshFactory.h>

#include "graphics/Mesh/GLMeshCube3D.h"

#include "graphics/Mesh/GLMeshSphere3D.h"

#include "graphics/Mesh/GLMeshCircle3D.h"

#include "graphics/Mesh/GLMeshSquare3D.h"

#include "graphics/Mesh/GLMeshTriangle3D.h"

#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

#include "Shaders/ShaderManager.h"

#include "Scene/SceneObjectFactory.h"

#include "graphics/Grid/GridSystem.h"

#include <graphics/GLTransformations/Transformations.h>

#include "graphics/Lighting/LightManager.h"

#include "graphics/Camera/Camera.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SCENE
{
	Scene::Scene(std::shared_ptr<GLgraphics::MeshData3D> data3D, Input::InputContext context)
		:meshData3D(data3D), inputContext(context)
	{
		DEBUG_PTR(meshData3D);
	}

    bool Scene::SetUpResources()
    {
		if (!m_sceneObjectFactory) {
			Logger::error("Scene can't setup m_sceneObjectFactory is nullptr!");
			return false;
		}

        auto testShader = m_shaderManager->getShaderInterface("testShader");
        if (!testShader) {
            Logger::warn("[Warning] testShader not found, using fallback");
            testShader = m_shaderManager->getShaderInterface("default");
            if (!testShader) {
                Logger::error("[Error] Fallback shader not found!");
                return false;
            }
        }

        auto lightShader = m_shaderManager->getShaderInterface("lightObjectShader");
        if (!lightShader) {
            Logger::warn("[Warning] lightObjectShader not found, using fallback");
            lightShader = m_shaderManager->getShaderInterface("default");
            if (!lightShader) {
                Logger::error("[Error] Fallback shader not found!");
                return false;
            }
        }

		// SUN
        for (int i = 0; i < 4; i++)
        {
            std::string name = "sphere_" + std::to_string(i);
            const auto sphere = m_sceneObjectFactory->createSphere(
                "gold",
                glm::vec3{ 30.0f, 30.0f, 15.0f },
                lightShader
            );
			sphere->setID( uniqueObjectIDGenerator() );
            sphere->setObjectName(name);
            sphere->addInputComponent(std::make_shared<InputComponent::SunInputComponent>(
                sphere->getTransform(), inputContext));
            AddObjectIntoScene(sphere);

			auto lightData = std::make_shared<LIGHTING::LightData>(sphere->getTransform()->getPosition());
			if (!lightData) {
				Logger::warn("[Scene::initResources] lightData is nullptr!");
				continue;
			}

			auto light = std::make_shared<LIGHTING::Light>(sphere, lightData);
			if (!light) {
				Logger::warn("[WARN] [Scene::initResources] light is nullptr, creating light object skipping!");
				continue;
			}

			// we are converting int to LightType for setting light type
			// in that case setLightType taking sequential values according to the loop
			light->setLightType(static_cast<LIGHTING::LightType>(i));

			getLightManager()->addLightToVec(light);
        }

		// CUBES
        for (int i = 0; i < 5; i++)
        {
            std::string name = "cube_" + std::to_string(i);
            const auto cube = m_sceneObjectFactory->createCube(
                "black plastic",
                glm::vec3{ 0.0f, 0.0f, i * 7.5f },
                testShader
            );
			cube->setID(uniqueObjectIDGenerator());
            cube->setObjectName(name);
            cube->getTransform()->setScale(glm::vec3{ 2.5f, 3.5f, 2.5f });

            if (i < 1) {
                cube->addInputComponent(std::make_shared<InputComponent::CubeInputComponent>(
                    cube->getTransform(), inputContext));
            }
            AddObjectIntoScene(cube);
        }

		// Floor
        const auto floor = m_sceneObjectFactory->createCube(
            "gold",
            glm::vec3{ 0.0f, -2.5f, 0.0f },
            testShader
        );
		floor->setID(uniqueObjectIDGenerator());
		floor->setObjectName("floor");
        floor->getTransform()->setScale(glm::vec3{ 50.5f, 1.5f, 50.5f });
        AddObjectIntoScene(floor);

        Logger::info("[Scene] SetUpResources successful!");
        return true;
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

	void Scene::drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		//std::cout << "sceneobject size: " << sceneObjects.size() << "\n";
		DEBUG_PTR(renderData);

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

		for (auto& obj : sceneObjects) {
			
			if (obj)
				obj->draw(view, projection, renderData);
			else
				Logger::warn("trying to draw an object that returns nullptr!");
		}

		// DEBUG MODE
		//for (auto& obj : sceneObjects) {
		//	try {
		//		if (obj) {
		//			obj->draw(view, projection, renderData);
		//		}
		//		else {
		//			throw std::runtime_error("trying to draw an object that returns nullptr!");
		//			// using throw can be costly (especially in per-frame loops)
		//		}
		//	}
		//	catch (const std::exception& e) {
		//		Logger::error("Error drawing object " + obj->getObjectName() + ": " + e.what());
		//		continue;
		//	}
		//}
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

	void Scene::updateAllObjects()
	{
		//std::cout << "[Scene] Updating all objects\n";

		// Updating Input components on the scene
		for (auto& obj : sceneObjects) {
			obj->update(*this);
		}
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


}