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

namespace MATERIAL
{
	class MaterialLibrary;
	struct Material;
}

namespace Input { class IInputComponent; };

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

		bool SetUpResources(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);

		void giveYourOwnMaterialsToObjects(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);

		bool initGrid(std::shared_ptr<GLgraphics::RenderData> renderData);
		void setOwnershipGridSystemToScene(std::unique_ptr<GRID::GridSystem>& gridSystem) { m_gridSystem = std::move(gridSystem); };

		void setLightManager(std::shared_ptr<LIGHTING::LightManager>& lightManager) { m_lightManager = lightManager; };
		std::shared_ptr<LIGHTING::LightManager>& getLightManager() { return m_lightManager; };

		void setShaderManager(std::shared_ptr<SHADER::ShaderManager> shaderManager) { m_shaderManager = shaderManager; };

		void debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);
		void drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);
		void drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void createLightObjects(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);

		void AddObjectIntoScene(const std::shared_ptr<SceneObject>& object);

		bool checkObjectIsAvailable(const std::string& name);

		std::vector<std::shared_ptr<SceneObject>>& getSceneObjectVector() { return sceneObjects; };

		void AddSceneObjectIntoMap(const std::string& name, const std::shared_ptr<SceneObject> object);
		std::shared_ptr<SceneObject> getSpesificSceneObjectWithName(const std::string& name);

		std::shared_ptr<LIGHTING::Light>& getLight();

		uint32_t uniqueObjectIDGenerator();

		void addInputComponent(std::shared_ptr<Input::IInputComponent> component);
		void updateInputComponents();

	private:
		const std::shared_ptr<GLgraphics::MeshData3D> meshData3D;
		std::vector<std::shared_ptr<SceneObject>> sceneObjects;
		Input::InputContext inputContext;

		std::shared_ptr<LIGHTING::LightManager> m_lightManager;
		
		std::shared_ptr<SHADER::ShaderManager> m_shaderManager;

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		std::vector<std::shared_ptr<Input::IInputComponent>> m_inputComponents;

		SceneObjectFactory* m_sceneObjectFactory = nullptr;

		std::unordered_map<std::string, std::shared_ptr<SceneObject>> sceneObjectsMap;

		uint32_t m_uniqueID;
	};
}