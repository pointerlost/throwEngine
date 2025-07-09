#pragma once
#include <glm/ext.hpp>
#include <vector>
#include <iostream>

namespace GLgraphics
{
	class RenderData;
	class IMesh;
	class Transformations;
}

namespace SHADER
{
	class ShaderManager;
	class IShader;
	class Shader;
};

namespace MATERIAL
{
	class MaterialLibrary;
	struct Material;
};

namespace InputComponent { class IInputComponent; };


namespace SCENE
{
	class Scene;

	class SceneObject
	{
	public:
		SceneObject(const std::shared_ptr<GLgraphics::IMesh> mesh, const std::string& name,
			const std::string& materialName);
		~SceneObject() = default;

		bool validateRenderState(const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void initializeMaterial(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);
		void setMaterialInstance(std::shared_ptr<MATERIAL::Material> instance);
		std::shared_ptr<MATERIAL::Material> getMaterialInstance() const { return m_materialInstance; };
		std::string getMaterialName() const { return m_materialName; };

		void setShaderInterface(const std::shared_ptr<SHADER::IShader>& shader) { m_shaderInterface = shader; };
		std::shared_ptr<SHADER::IShader> getShaderInterface() const { return m_shaderInterface; };

		void draw(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void update(SCENE::Scene &scene);

		void setID(uint32_t id) { m_objectID = id;   };
		uint32_t getID() const  { return m_objectID; };

		void addInputComponent(std::shared_ptr<InputComponent::IInputComponent> component);

		void setObjectName(const std::string& name) { this->m_objectName = name; };
		const std::string& getObjectName() const { return m_objectName; };

		std::shared_ptr<GLgraphics::Transformations>& getTransform() { return m_transform; };

	private:
		std::shared_ptr<GLgraphics::IMesh> m_mesh;
		std::shared_ptr<GLgraphics::Transformations> m_transform;
		std::vector<std::shared_ptr<InputComponent::IInputComponent>> inputComponents;

		std::shared_ptr<SHADER::IShader> m_shaderInterface = nullptr;
		std::string m_objectName;
		std::string m_materialName;

		std::shared_ptr<MATERIAL::Material> m_materialInstance;

		uint32_t m_objectID = -1;
	};
}
