#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <unordered_map>
#include <nlohmann/json.hpp>

// Forward declaration
using json = nlohmann::json;
namespace TEXTURE { class Textures; };

namespace MATERIAL
{

	struct Material
	{
		std::string m_name;

		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;

		bool m_hasDiffuseTexture;
		bool m_hasSpecularTexture;

		uint32_t m_diffuseTextureID;
		uint32_t m_specularTextureID;

		Material() = default;
	};


	class MaterialLibrary
	{
	public:
		MaterialLibrary() = default;

		bool contains(const std::string& name) const noexcept { return m_nameToIndex.find(name) != m_nameToIndex.end(); };

		bool loadMaterialFromJSON(const std::string& filePath, const std::shared_ptr<TEXTURE::Textures>& textureManager);
		bool addMaterial(const std::shared_ptr<Material>& material);

		std::pair<uint32_t, bool> loadTextureField(const json& j, const std::string& field, const std::shared_ptr<TEXTURE::Textures>& texManager);

		std::shared_ptr<Material> createMaterial(const json& j, const std::shared_ptr<TEXTURE::Textures>& textureManager);

		json parseJSONfile(const std::string& filePath);
		glm::vec3 parseColor(const json& j, const std::string& field, const glm::vec3& fallback);

		std::shared_ptr<Material> getDefaultMaterial() { return m_defaultMaterial; };

		//std::shared_ptr<const Material> getMaterialByNameConst(const std::string& name) const noexcept;
		//std::shared_ptr<const Material> getMaterialByIndexConst(size_t index) const noexcept;

		std::shared_ptr<Material> getMaterialByName(const std::string& name);
		std::shared_ptr<Material> getMaterialByIndex(int index);

	private:
		std::vector<std::shared_ptr<Material>> m_materials;
		std::unordered_map<std::string, size_t> m_nameToIndex;

		std::shared_ptr<Material> m_defaultMaterial;
		void createDefaultMaterials();
	};
}