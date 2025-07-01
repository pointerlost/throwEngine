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

    std::shared_ptr<SceneObject> SceneObjectFactory::createSun(
        const std::string& materialName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> shader)
    {
        auto sun = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshSphere3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("sphere")),
            "sun_" + std::to_string(rand()),
            materialName
        );
        sun->setShaderInterface(shader);
        sun->getTransform()->setPosition(position);
        sun->getTransform()->setScale(glm::vec3(2.0f)); // Sun is typically larger
        return sun;
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
        cube->setShaderInterface(shader);
        cube->getTransform()->setPosition(position);
        return cube;
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createSphere(
        const std::string& materialName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> shader)
    {
        auto sphere = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshSphere3D>(m_pImpl->meshData, m_pImpl->meshData->getObjectInfo("sphere")),
            "sphere_",
            materialName
        );
        sphere->setShaderInterface(shader);
        sphere->getTransform()->setPosition(position);
        return sphere;
    }

    std::shared_ptr<LIGHTING::Light> SceneObjectFactory::createPointLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        auto lightObject = createSphere("emissive", position, lightShader);
        lightObject->setObjectName(objectName);
        lightObject->getTransform()->setScale(glm::vec3(0.5));

        // create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);
        lightData->setDiffuse(glm::vec3(1.0, 0.9, 0.8));
        lightData->setConstant(1.0);
        lightData->setLinear(0.09);
        lightData->setQuadratic(0.032);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Point);

        return light;
    }

    std::shared_ptr<LIGHTING::Light> SceneObjectFactory::createDirectionalLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        // Create visual representation (sun)
        auto lightObject = createSun("emissive", position, lightShader);
        lightObject->setObjectName(objectName);

        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);
        lightData->setDiffuse(glm::vec3(1.0f, 0.95f, 0.9f)); // Sunlight color

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::Directional);

        return light;
    }

    std::shared_ptr<LIGHTING::Light> SceneObjectFactory::createSpotLight(
        const std::string& objectName,
        const glm::vec3& position,
        std::shared_ptr<SHADER::IShader> lightShader,
        std::shared_ptr<MATERIAL::MaterialLibrary> materialLib)
    {
        // Create visual representation (cone-shaped object)
        auto lightObject = createSphere("emissive", position, lightShader);
        lightObject->setObjectName(objectName);
        lightObject->getTransform()->setScale(glm::vec3(0.5f, 1.5f, 0.5f)); // Elongated sphere for spotlight

        // Create light component
        auto lightData = std::make_shared<LIGHTING::LightData>(position);
        lightData->setDiffuse(glm::vec3(1.0f, 1.0f, 0.9f)); // Bright white-yellow light
        lightData->setConstant(1.0f);
        lightData->setLinear(0.09f);
        lightData->setQuadratic(0.032f);

        auto light = std::make_shared<LIGHTING::Light>(lightObject, lightData);
        light->setLightType(LIGHTING::LightType::SpotLight);

        return light;
    }
}