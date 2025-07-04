#pragma once
#include <iostream>
#include <vector>
#include "glm/ext.hpp"
#include "Input/InputContext.h"
#include <unordered_map>
#include "graphics/Grid/GridSystem.h"

namespace GLgraphics
{
	class RenderData;
	class MeshData3D;
};

namespace LIGHTING
{
	class Light;
	class LightData;
	class LightManager;
}

namespace SHADER
{
	class ShaderManager;
}

namespace SCENE
{
	// Forward declarations
	class SceneObject;
	class SceneObjectFactory;

	class Scene
	{
	public:
		Scene(std::shared_ptr<GLgraphics::MeshData3D> data, Input::InputContext context);

		void setSceneObjectFactoryPointer(SceneObjectFactory* factory) { m_sceneObjectFactory = factory; };

		bool SetUpResources();

		bool initGrid(std::shared_ptr<GLgraphics::RenderData> renderData);
		void setOwnershipGridSystemToScene(std::unique_ptr<GRID::GridSystem>& gridSystem) { m_gridSystem = std::move(gridSystem); };

		void setLightManager(std::shared_ptr<LIGHTING::LightManager>& lightManager) { m_lightManager = lightManager; };
		std::shared_ptr<LIGHTING::LightManager>& getLightManager() { return m_lightManager; };

		void setShaderManager(std::shared_ptr<SHADER::ShaderManager> shaderManager) { m_shaderManager = shaderManager; };

		void drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void AddObjectIntoScene(const std::shared_ptr<SceneObject>& object);
		void updateAllObjects();

		bool checkObjectIsAvailable(const std::string& name);

		std::vector<std::shared_ptr<SceneObject>> getSceneObjectVector() const { return sceneObjects; };

		void AddSceneObjectIntoMap(const std::string& name, const std::shared_ptr<SceneObject> object);
		std::shared_ptr<SceneObject> getSpesificSceneObjectWithName(const std::string& name);

		uint32_t uniqueObjectIDGenerator();

	private:
		const std::shared_ptr<GLgraphics::MeshData3D> meshData3D;
		std::vector<std::shared_ptr<SceneObject>> sceneObjects;
		Input::InputContext inputContext;

		std::shared_ptr<LIGHTING::LightManager> m_lightManager;
		
		std::shared_ptr<SHADER::ShaderManager> m_shaderManager;

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		SceneObjectFactory* m_sceneObjectFactory = nullptr;

		std::unordered_map<std::string, std::shared_ptr<SceneObject>> sceneObjectsMap;

		uint32_t m_uniqueID;
	};
}