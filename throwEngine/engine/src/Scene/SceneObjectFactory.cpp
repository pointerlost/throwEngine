#include "Scene/SceneObjectFactory.h"

#include "graphics/Material/MaterialLib.h"

#include "Scene/SceneObject.h"

#include "graphics/Lighting/LightData.h"

#include <Shaders/BasicShader.h>
#include <Shaders/LightShader.h>

#include "graphics/Lighting/Light.h"

#include "graphics/Mesh/GLMeshCircle3D.h"
#include "graphics/Mesh/GLMeshCube3D.h"
#include "graphics/Mesh/GLMeshSphere3D.h"
#include "graphics/Mesh/GLMeshSquare3D.h"
#include "graphics/Mesh/GLMeshTriangle3D.h"
#include "graphics/Mesh/GLMeshFactory.h"
#include "graphics/Mesh/GLMesh3D.h"

#include <Input/InputComponent.h>

#include <Input/InputComponentFactory.h>

#include "graphics/GLTransformations/Transformations.h"

#include <core/Logger.h>


namespace SCENE
{

    struct SceneObjectFactory::Impl
    {
        std::unique_ptr<GLgraphics::MeshFactory> meshFactory;
        std::shared_ptr<GLgraphics::MeshData3D> meshData;
        std::unique_ptr<GLgraphics::Mesh> meshManager;

        void initBaseMeshes() {
            auto addMesh = [&](const std::string& name) {
                auto [vertices, indices] = meshFactory->createMeshObject(name);
                meshData->AddMesh3DToMeshData(name, vertices, indices);
                };
            addMesh("cube");
            addMesh("sphere");
            addMesh("triangle");
            addMesh("square");
            addMesh("circle");
        }
    };

	SceneObjectFactory::SceneObjectFactory()
        :m_pImpl(std::make_unique<Impl>())
	{
		m_pImpl->meshFactory = std::make_unique<GLgraphics::MeshFactory>();
		m_pImpl->meshData = std::make_shared<GLgraphics::MeshData3D>();
		m_pImpl->initBaseMeshes();
		m_pImpl->meshManager = std::make_unique<GLgraphics::Mesh>(m_pImpl->meshData);
	}

    SceneObjectFactory::~SceneObjectFactory() = default; // Impl is complete here

    std::shared_ptr<GLgraphics::MeshData3D> SceneObjectFactory::getMeshData() const
    {
        return m_pImpl->meshData;
    }

    void SceneObjectFactory::initBaseMeshes() {
		// This function initializes the base meshes used in the scene.
        auto addMesh = [&](const std::string& name) {
            auto [vertices, indices] = m_pImpl->meshFactory->createMeshObject(name);
            m_pImpl->meshData->AddMesh3DToMeshData(name, vertices, indices);
        };

		// Add predefined meshes to the mesh data
        addMesh("cube");
        addMesh("sphere");
        addMesh("triangle");
        addMesh("square");
        addMesh("circle");
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createCube(
        const std::string& materialName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> shader)
    {
		auto id = uniqueObjectIDGenerator();

        auto cube = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshCube3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("cube")),
            "cube_" + std::to_string(id),  // Unique name
            materialName
        );

		// Create input component for the cube
        auto inputComponent = Input::InputComponentFactory::createObjectComponent(
            Input::InputType::CubeInputComponent,
            cube->getTransform()
		);

        if (inputComponent) {
			cube->setInputComponent(inputComponent);
        } else {
            Logger::warn("[SceneObjectFactory::createCube] Input component creation failed.");
		}

		// Set the cube's properties
        cube->setID(id);
        cube->setShaderInterface(shader);
        cube->getTransform()->setPosition(position);
        cube->getTransform()->setScale(glm::vec3{ 7.5f, 7.5f, 7.5f });
        cube->setobjectType(ObjectType::Visual); // Set the object type to Visual
        return cube;
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createSphere(
        const std::string& materialName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> shader)
    {
		auto id = uniqueObjectIDGenerator();
		// Create a sphere object with the specified material and position
        auto sphere = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshSphere3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("sphere")),
			"sphere_" + std::to_string(id),  // Unique name
            materialName
        );

        // Create input component for the cube
        auto inputComponent = Input::InputComponentFactory::createObjectComponent(
            Input::InputType::SphereInputComponent,
            sphere->getTransform()
        );

        if (inputComponent) {
            sphere->setInputComponent(inputComponent);
        }
        else {
            Logger::warn("[SceneObjectFactory::createSphere] Input component creation failed.");
        }
        
        sphere->setID(id);
        sphere->setObjectName("sphere_" + std::to_string(id));
        sphere->setShaderInterface(shader);
        sphere->getTransform()->setPosition(position);
        sphere->getTransform()->setScale(glm::vec3{ 3.5f, 3.5f, 3.5f });
        sphere->setobjectType(ObjectType::Visual); // Set the object type to Visual
        return sphere;
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createPointLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
		// Create visual representation (sphere)
        auto lightObject = createSphere("gold", position, lightShader);

        if (!lightObject) {
            Logger::error("[SceneObjectFactory::createPointLight] Failed to create light object.");
            return { nullptr, nullptr };
		}

        lightObject->setShaderInterface(lightShader);

        if (auto lightShaderPtr = std::dynamic_pointer_cast<SHADER::LightShader>(lightShader)) {
            lightShaderPtr->setShaderInterface(lightObject);
        }
        else {
            Logger::warn("[SceneObjectFactory::createSpotLight] Failed to cast to LightShader.");
        }

        lightObject->setObjectName("point_" + std::to_string(lightObject->getID()));
        lightObject->setobjectType(ObjectType::LightVisual);

        // create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Point);
        light->setSceneObjectID(lightObject->getID());

        // we have to create a new input component for the light object overwriting the existing one
        auto inputComponent = Input::InputComponentFactory::createLightComponent(
            Input::InputType::LightInputComponent,
            lightObject->getTransform(),
            light
        );

        if (inputComponent) {
            lightObject->setInputComponent(inputComponent);
        }
        else {
            Logger::warn("[SceneObjectFactory::createPointLight] Input component creation failed.");
        }
		
        lightObject->setLightSource(light);

        return { light, lightObject };
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createDirectionalLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        // Create visual representation (sun)
        auto lightObject = createSphere("gold", position, lightShader);

        if (!lightObject) {
            Logger::error("[SceneObjectFactory::createDirectionalLight] Failed to create light object.");
            return { nullptr, nullptr };
		}

        lightObject->setShaderInterface(lightShader);

        if (auto lightShaderPtr = std::dynamic_pointer_cast<SHADER::LightShader>(lightShader)) {
            lightShaderPtr->setShaderInterface(lightObject);
        }
        else {
            Logger::warn("[SceneObjectFactory::createSpotLight] Failed to cast to LightShader.");
        }

        lightObject->setObjectName("directional_" + std::to_string(lightObject->getID()));
        lightObject->getTransform()->setScale(glm::vec3(0.5f, 1.5f, 0.5f)); // Elongated sphere for spotlight
        lightObject->setobjectType(ObjectType::LightVisual);
        
        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Directional);
        light->setSceneObjectID(lightObject->getID());

        // we have to create a new input component for the light object overwriting the existing one
        auto inputComponent = Input::InputComponentFactory::createLightComponent(
            Input::InputType::LightInputComponent,
            lightObject->getTransform(),
            light
        );

        if (inputComponent) {
            lightObject->setInputComponent(inputComponent);
        }
        else {
            Logger::warn("[SceneObjectFactory::createDirectionalLight] Input component creation failed.");
        }

		lightObject->setLightSource(light);

        return { light, lightObject };
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createSpotLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        // Create visual representation (cone-shaped object)
        auto lightObject = createSphere("gold", position, lightShader);

        if (!lightObject) {
            Logger::error("[SceneObjectFactory::createSpotLight] Failed to create light object.");
            return { nullptr, nullptr };
        }

        lightObject->setShaderInterface(lightShader);

        if (auto lightShaderPtr = std::dynamic_pointer_cast<SHADER::LightShader>(lightShader)) {
            lightShaderPtr->setShaderInterface(lightObject);
        } else {
            Logger::warn("[SceneObjectFactory::createSpotLight] Failed to cast to LightShader.");
		}

        lightObject->setObjectName("spot_" + std::to_string(lightObject->getID()));
        lightObject->setobjectType(ObjectType::LightVisual);

        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Spot);
		light->setSceneObjectID(lightObject->getID());

        auto inputComponent = Input::InputComponentFactory::createLightComponent(
            Input::InputType::LightInputComponent,
            lightObject->getTransform(),
            light
        );

        if (inputComponent) {
            lightObject->setInputComponent(inputComponent);
        }
        else {
            Logger::warn("[SceneObjectFactory::createSpotLight] Input component creation failed.");
        }

		lightObject->setLightSource(light);

		return { light, lightObject };
    }

    uint32_t SceneObjectFactory::uniqueObjectIDGenerator()
    {
        static uint32_t uniqueID = 0;
        return uniqueID++;
	}
    
}