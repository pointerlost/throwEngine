#include "graphics/Material/MaterialLib.h"

#include "core/File.h"

#include "graphics/Textures/Textures.h"

#include "Config.h"

#include <stdexcept>

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = core::File::get();

namespace MATERIAL
{

	bool MaterialLibrary::loadMaterialFromJSON(const std::string& filePath, const std::shared_ptr<TEXTURE::Textures>& textureManager)
    {
        json j = parseJSONfile(filePath);
        if (!j.contains("materials") || !j["materials"].is_array()) {
            Logger::error("The file format is incorrect.");
            return false;
        }

        m_materials.clear();
        m_nameToIndex.clear();
        createDefaultMaterials();

        for (const auto& item : j["materials"]) {
            auto mat = createMaterial(item, textureManager);
            if (!addMaterial(mat)) {
                Logger::warn("Material with the same name already exists: " + mat->m_name);
            }
        }

        return true;
    }

	bool MaterialLibrary::addMaterial(const std::shared_ptr<Material>& material)
	{
		if (!material) return false;
		if (contains(material->m_name)) return false;

		m_nameToIndex[material->m_name] = m_materials.size();
		m_materials.push_back(material);
		return true;
	}

    std::pair<uint32_t, bool> MaterialLibrary::loadTextureField(const json& j, const std::string& field, const std::shared_ptr<TEXTURE::Textures>& texManager)
    {
        if (j.contains(field) && j[field].is_array() && !j[field].empty()) {
            std::string path = std::string(ASSETS_DIR) + "/" + j[field][0].get<std::string>();
            if (!std::filesystem::exists(path))
                Logger::error("Texture file NOT FOUND" + path);
            else {
                Logger::info("Texture file FOUND" + path);
                return { texManager->generateTexture(path), true };
            }
        }
        return { 0, false };
    }

    std::shared_ptr<MATERIAL::Material> MaterialLibrary::createMaterial(const json& data, const std::shared_ptr<TEXTURE::Textures>& textureManager)
    {
        auto mat = std::make_shared<MATERIAL::Material>();
        mat->m_name = data.value("name", "unnamed");

        mat->m_ambient   = parseColor(data, "ambient",  glm::vec3(0.1f));
        mat->m_diffuse   = parseColor(data, "diffuse",  glm::vec3(0.8f));
        mat->m_specular  = parseColor(data, "specular", glm::vec3(0.5f));
        mat->m_shininess = data.value("shininess", 32.0f);

        // load diffuse texture when object has a texture
        std::tie(mat->m_diffuseTextureID,  mat->m_hasDiffuseTexture)  = loadTextureField(data, "diffuseTexture", textureManager);

        // load specular texture when object has a texture
        std::tie(mat->m_specularTextureID, mat->m_hasSpecularTexture) = loadTextureField(data, "specularTexture", textureManager);

        return mat;
    }

    json MaterialLibrary::parseJSONfile(const std::string& filePath)
    {
        std::string content = file.readFromFile(filePath);
        if (content.empty()) {
            Logger::error("MaterialLibrary: empty or missing file: " + filePath);
            return {};
        }

        try {
            return json::parse(content);
        }
        catch (const json::parse_error& e) {
            Logger::error("MaterialLibrary: JSON parse error in " + filePath + ": " + e.what());
            return {};
        }
    }

    glm::vec3 MaterialLibrary::parseColor(const json& j, const std::string& field, const glm::vec3& fallback)
    {
        if (j.contains(field) && j[field].is_array() && j[field].size() == 3)
            return glm::vec3(j[field][0], j[field][1], j[field][2]);
        return fallback;
    }

	std::shared_ptr<Material> MaterialLibrary::getMaterialByName(const std::string& name)
	{
		auto it = m_nameToIndex.find(name);
		if (it == m_nameToIndex.end()) {
			Logger::warn("Material not found: " + name);
			return getDefaultMaterial();
		}
		return m_materials[it->second];
	}

	std::shared_ptr<Material> MaterialLibrary::getMaterialByIndex(int index)
	{
		if (index >= m_materials.size()) {
			Logger::warn("Material index out of range: " + std::to_string(index));
			return getDefaultMaterial();
		}
		return m_materials[index];
	}

	void MaterialLibrary::createDefaultMaterials()
	{
		if (m_defaultMaterial) return;

		m_defaultMaterial = std::make_shared<Material>();
		m_defaultMaterial->m_name = "default";
		m_defaultMaterial->m_ambient = glm::vec3(1.0f, 0.0f, 1.0f); // Bright purple (error color)
		m_defaultMaterial->m_diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
		m_defaultMaterial->m_specular = glm::vec3(0.5f);
		m_defaultMaterial->m_shininess = 32.0f;
		m_defaultMaterial->m_hasDiffuseTexture = false;
		m_defaultMaterial->m_hasSpecularTexture = false;

		// ensure default material is always available
		addMaterial(m_defaultMaterial);
	}
}