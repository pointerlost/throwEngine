#pragma once
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include <memory>

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

namespace Input { class IInputComponent; };

namespace LIGHTING
{
	class Light;
	struct LightColor;
};

namespace SCENE
{
	class Scene;


	enum class ObjectType
	{
		Visual,
		LightVisual
	};


	class SceneObject
	{
	public:
		SceneObject(const std::shared_ptr<GLgraphics::IMesh> mesh, const std::string& name,
			const std::string& materialName);
		~SceneObject() = default;

		bool validateRenderState(const std::shared_ptr<GLgraphics::RenderData>& renderData);
		bool validateRenderStateLight(const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void initializeMaterial(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);
		void setMaterialInstance(std::shared_ptr<MATERIAL::Material> instance);
		std::shared_ptr<MATERIAL::Material> getMaterialInstance() const { return m_materialInstance; };
		std::string getMaterialName() const { return m_materialName; };

		void setShaderInterface(const std::shared_ptr<SHADER::IShader> shader);
		std::shared_ptr<SHADER::IShader> getShaderInterface() const { return m_shaderInterface; };

		void setLightSource(const std::shared_ptr<LIGHTING::Light>& light) { m_lightSource = light; };
		std::shared_ptr<LIGHTING::Light> getLightSource() const { return m_lightSource; };

		void draw(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);
		void drawLight(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void setID(uint32_t id) { m_objectID = id;   };
		uint32_t getID() const  { return m_objectID; };

		void setObjectName(const std::string& name) { this->m_objectName = name; };
		const std::string& getObjectName() const { return m_objectName; };

		void setInputComponent(const std::shared_ptr<Input::IInputComponent>& inputComponent) { m_inputComponent = inputComponent; };
		std::shared_ptr<Input::IInputComponent> getInputComponent() const { return m_inputComponent; };

		std::shared_ptr<GLgraphics::Transformations>& getTransform() { return m_transform; };

		void setobjectType(ObjectType type) { m_objectType = type; };
		ObjectType getObjectType() const { return m_objectType; };

	private:
		std::shared_ptr<GLgraphics::IMesh> m_mesh;
		std::shared_ptr<GLgraphics::Transformations> m_transform;

		std::shared_ptr<Input::IInputComponent> m_inputComponent = nullptr;
		std::shared_ptr<SHADER::IShader> m_shaderInterface = nullptr;

		std::string m_objectName;
		std::string m_materialName;

		ObjectType m_objectType = ObjectType::Visual; // type of the object, visual or light visual

		std::shared_ptr<LIGHTING::Light> m_lightSource = nullptr; // if this object is a light source

		std::shared_ptr<MATERIAL::Material> m_materialInstance;

		uint32_t m_objectID = -1;
	};
}
