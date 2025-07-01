#include "graphics/Material/MaterialLib.h"

#include "core/File.h"

#include "graphics/Textures/Textures.h"

#include <stdexcept>

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

using json = nlohmann::json;

static auto& file = core::File::get();

namespace MATERIAL
{

	bool MaterialLibrary::loadMaterialFromJSON(const std::string& filePath, const std::shared_ptr<TEXTURE::Textures>& textureManager)
    {
        std::string content = file.readFromFile(filePath);
        if (content.empty()) {
            Logger::error("MaterialLibrary: Empty or missing file: " + filePath);
            return false;
        }

        json j;
        try {
            j = json::parse(content);
        }
        catch (const json::parse_error& e) {
            Logger::error("MaterialLibrary: JSON parse error in " + filePath + ": " + e.what());
            return false;
        }

        if (!j.contains("materials") || !j["materials"].is_array()) {
            Logger::error("MaterialLibrary: Invalid JSON format in " + filePath);
            return false;
        }

        // Clear existing materials
        m_materials.clear();
        m_nameToIndex.clear();

        // Create default material first
        createDefaultMaterials();

        // Load materials
        for (const auto& m : j["materials"]) {
            try {
                auto mat = std::make_shared<Material>();
                mat->m_name = m.value("name", "unnamed_" + std::to_string(m_materials.size()));

                // Parse color values with safety checks
                auto parseColor = [&](const std::string& field, glm::vec3 defaultVal) {
                    if (m.contains(field) && m[field].is_array() && m[field].size() == 3) {
                        return glm::vec3(m[field][0], m[field][1], m[field][2]);
                    }
                    return defaultVal;
                    };

                mat->m_ambient = parseColor("ambient", glm::vec3(0.1f));
                mat->m_diffuse = parseColor("diffuse", glm::vec3(0.8f));
                mat->m_specular = parseColor("specular", glm::vec3(0.5f));
                mat->m_shininess = m.value("shininess", 32.0f);

                // Load textures with error handling
                auto loadTexture = [&](const std::string& field) -> std::pair<uint32_t, bool> {
                    if (m.contains(field) && m[field].is_array() && !m[field].empty()) {
                        try {
                            std::string texPath = m[field][0];
                            uint32_t texID = textureManager->generateTexture(texPath);
                            return { texID, true };
                        }
                        catch (...) {
                            Logger::warn("Failed to load texture for material " + mat->m_name);
                        }
                    }
                    return { 0, false };
                    };

                auto [diffuseID, hasDiffuse] = loadTexture("diffuseTexture");
                mat->m_diffuseTextureID = diffuseID;
                mat->m_hasDiffuseTexture = hasDiffuse;

                auto [specularID, hasSpecular] = loadTexture("specularTexture");
                mat->m_specularTextureID = specularID;
                mat->m_hasSpecularTexture = hasSpecular;

                if (!addMaterial(mat)) {
                    Logger::warn("Duplicate material name: " + mat->m_name);
                }
            }
            catch (const std::exception& e) {
                Logger::error("Error loading material: " + std::string(e.what()));
                continue; // Skip this material but continue loading others
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