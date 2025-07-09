#include "Scene/SceneObjectFactory.h"

#include "graphics/Material/MaterialLib.h"

#include "Scene/SceneObject.h"

#include "graphics/Lighting/LightData.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Mesh/GLMeshCircle3D.h"
#include "graphics/Mesh/GLMeshCube3D.h"
#include "graphics/Mesh/GLMeshSphere3D.h"
#include "graphics/Mesh/GLMeshSquare3D.h"
#include "graphics/Mesh/GLMeshTriangle3D.h"
#include "graphics/Mesh/GLMeshFactory.h"
#include "graphics/Mesh/GLMesh3D.h"

#include <Input/InputComponent.h>

#include "graphics/GLTransformations/Transformations.h"


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
        auto addMesh = [&](const std::string& name) {
            auto [vertices, indices] = m_pImpl->meshFactory->createMeshObject(name);
            m_pImpl->meshData->AddMesh3DToMeshData(name, vertices, indices);
        };

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
        auto cube = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshCube3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("cube")),
            "cube_" + std::to_string(rand()),  // Unique name
            materialName
        );
        Input::InputContext inputContext{};

        cube->addInputComponent(std::make_shared<InputComponent::SphereInputComponent>(
            cube->getTransform(), inputContext));
        cube->setID(uniqueObjectIDGenerator());
        cube->setShaderInterface(shader);
        cube->getTransform()->setPosition(position);
        cube->getTransform()->setScale(glm::vec3{ 7.5f, 7.5f, 7.5f });
        return cube;
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createSphere(
        const std::string& materialName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> shader)
    {
        auto sphere = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshSphere3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("sphere")),
			"sphere_" + std::to_string(rand()),  // Unique name
            materialName
        );
        Input::InputContext inputContext{};

		// Set up the sphere's transformation and input component
        sphere->addInputComponent(std::make_shared<InputComponent::SphereInputComponent>(
			sphere->getTransform(), inputContext ));
        sphere->setID(uniqueObjectIDGenerator());
        sphere->setObjectName("sphere_" + std::to_string((uniqueObjectIDGenerator())));
        sphere->setShaderInterface(shader);
        sphere->getTransform()->setPosition(position);
        sphere->getTransform()->setScale(glm::vec3{ 3.5f, 3.5f, 3.5f });
        return sphere;
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createPointLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        Input::InputContext inputContext{};

        auto lightObject = createSphere("gold", position, lightShader);
        lightObject->setObjectName("point_" + std::to_string((uniqueObjectIDGenerator())));
        lightObject->addInputComponent(std::make_shared<InputComponent::LightInputComponent>(
            lightObject->getTransform(), inputContext));

        // create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Point);

        return { light, lightObject };
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createDirectionalLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        Input::InputContext inputContext{};
        
        // Create visual representation (sun)
        auto lightObject = createSphere("gold", position, lightShader);
        lightObject->setObjectName("directional_" + std::to_string((uniqueObjectIDGenerator())));
        lightObject->getTransform()->setScale(glm::vec3(0.5f, 1.5f, 0.5f)); // Elongated sphere for spotlight
        lightObject->addInputComponent(std::make_shared<InputComponent::LightInputComponent>(
            lightObject->getTransform(), inputContext));

        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Directional);

        return { light, lightObject };
    }

    std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> SceneObjectFactory::createSpotLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        Input::InputContext inputContext{};

        // Create visual representation (cone-shaped object)
        auto lightObject = createSphere("gold", position, lightShader);
        lightObject->setObjectName("spot_" + std::to_string((uniqueObjectIDGenerator())));
        lightObject->addInputComponent(std::make_shared<InputComponent::LightInputComponent>(
			lightObject->getTransform(), inputContext));

        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::SpotLight);

		return { light, lightObject };
    }

    uint32_t SceneObjectFactory::uniqueObjectIDGenerator()
    {
        static uint32_t uniqueID = 0;
        m_uniqueObjectID = uniqueID++;

        return m_uniqueObjectID;
	}
}