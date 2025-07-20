#pragma once
#include <string>
#include <memory>
#include <glm/ext.hpp>

#include <Input/InputContext.h>

namespace SHADER
{
	class IShader;
}

namespace GLgraphics
{
	class MeshData3D;
}

namespace LIGHTING
{
	class Light;
	class LightData;
}

namespace MATERIAL { class MaterialLibrary; };

namespace SCENE
{
	class SceneObject;

	class SceneObjectFactory
	{
	public:
		SceneObjectFactory();
		~SceneObjectFactory();

		std::shared_ptr<GLgraphics::MeshData3D> getMeshData() const;

		std::shared_ptr<SceneObject> createCube(
			const std::string& materialName,
			const glm::vec3& position,
			std::shared_ptr<SHADER::IShader> shader);

		std::shared_ptr<SceneObject> createSphere(
			const std::string& materialName,
			const glm::vec3& position,
			std::shared_ptr<SHADER::IShader> shader);

		std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> createPointLight(
			const std::string& objectName,
			const glm::vec3& position,
			std::shared_ptr<SHADER::IShader> lightShader,
			std::shared_ptr<MATERIAL::MaterialLibrary> materialLib
		);

		std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> createDirectionalLight(
			const std::string& objectName,
			const glm::vec3& position,
			std::shared_ptr<SHADER::IShader> lightShader,
			std::shared_ptr<MATERIAL::MaterialLibrary> materialLib
		);

		std::pair<std::shared_ptr<LIGHTING::Light>, std::shared_ptr<SceneObject>> createSpotLight(
			const std::string& objectName,
			const glm::vec3& position,
			std::shared_ptr<SHADER::IShader> lightShader,
			std::shared_ptr<MATERIAL::MaterialLibrary> materialLib
		);

		uint32_t uniqueObjectIDGenerator();

		Input::InputContext createInputContext() {
			Input::InputContext inputContext{};
			inputContext.m_rotationSpeed = 0.5f;
			inputContext.m_delta = 0.016f; // Assuming 60 FPS
			inputContext.m_angle = 0.0f;
			inputContext.m_radius = 45.0f;
			return inputContext;
		};

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;
		
		void initBaseMeshes();
	};
}