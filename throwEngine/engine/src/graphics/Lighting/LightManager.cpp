#include "graphics/Lighting/LightManager.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include "Shaders/ShaderProgram.h"

#include <Scene/SceneObject.h>

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace LIGHTING
{
    void LightManager::uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader) const
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }

        shader->bind();

        // Check which lighting mode the shader supports
        bool useArray = shader->hasUniform("lights[0].position");
        bool useSingle = shader->hasUniform("light.position");

        if (useArray) {
            // Array-style lighting
            for (size_t i = 0; i < m_lightsVec.size() && i < MAX_LIGHTS; ++i) {
                const auto& light = m_lightsVec[i];
                std::string prefix = "lights[" + std::to_string(i) + "]";

                // Position and direction
                shader->hasUniform(prefix + ".position")
                    ? shader->setVec3(prefix + ".position", light->getPosition())
                    : Logger::warn("[LightManager::uploadLights] shader has no position uniform!");
                
                shader->hasUniform(prefix + ".direction")
                    ? shader->setVec3(prefix + ".direction", light->getDirection())
                    : Logger::warn("[LightManager::uploadLights] shader has no direction uniform!");

                // Colors
                shader->hasUniform(prefix + ".diffuse")
                    ? shader->setVec3(prefix + ".diffuse", light->getLightDiffuse())
                    : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!");

                shader->hasUniform(prefix + ".specular")
                    ? shader->setVec3(prefix + ".specular", light->getLightSpecular())
                    : Logger::warn("[LightManager::uploadLights] shader has no specular uniform!");

                // Attenuation
                shader->hasUniform(prefix + ".constant")
                    ? shader->setFloat(prefix + ".constant", light->getConstant())
                    : Logger::warn("[LightManager::uploadLights] shader has no constant uniform!");
                
                shader->hasUniform(prefix + ".linear")
                    ? shader->setFloat(prefix + ".linear", light->getLinear())
                    : Logger::warn("[LightManager::uploadLights] shader has no linear uniform!");

                shader->hasUniform(prefix + ".quadratic")
                    ? shader->setFloat(prefix + ".quadratic", light->getQuadratic())
                    : Logger::warn("[LightManager::uploadLights] shader has no quadratic uniform!");

                // Spotlight
                shader->hasUniform(prefix + ".cutOff")
                    ? shader->setFloat(prefix + ".cutOff", light->getCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no cutOff uniform!");

                shader->hasUniform(prefix + ".outerCutOff")
                    ? shader->setFloat(prefix + ".outerCutOff", light->getOuterCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no outerCutOff uniform!");
                
                shader->hasUniform(prefix + ".type")
                    ? shader->setFloat(prefix + ".type", static_cast<int>(light->getType()))
                    : Logger::warn("[LightManager::uploadLights] shader has no type uniform!");
            }
            if (shader->hasUniform("numLights"))
                shader->setInt("numLights", static_cast<int>(m_lightsVec.size()));
            else
                Logger::warn("[LightManager::uploadLights] shader has no numLights uniform!");
        }
        else if (useSingle && !m_lightsVec.empty()) {
            // Single light fallback (use first light)
            const auto& light = m_lightsVec[0];

            // Position and direction
            shader->hasUniform("light.position")
                ? shader->setVec3("light.position", light->getPosition())
                : Logger::warn("[LightManager::uploadLights] shader has no light.position uniform!");

            shader->hasUniform("light.direction")
                ? shader->setVec3("light.direction", light->getDirection())
                : Logger::warn("[LightManager::uploadLights] shader has no light.direction uniform!");

            // Colors
            shader->hasUniform("light.diffuse")
                ? shader->setVec3("light.diffuse", light->getLightDiffuse())
                : Logger::warn("[LightManager::uploadLights] shader has no light.diffuse uniform!");

            shader->hasUniform("light.specular")
                ? shader->setVec3("light.specular", light->getLightSpecular())
                : Logger::warn("[LightManager::uploadLights] shader has no light.specular uniform!");

            // Attenuation
            shader->hasUniform("light.constant")
                ? shader->setFloat("light.constant", light->getConstant())
                : Logger::warn("[LightManager::uploadLights] shader has no light.constant uniform!");

            shader->hasUniform("light.linear")
                ? shader->setFloat("light.linear", light->getLinear())
                : Logger::warn("[LightManager::uploadLights] shader has no light.linear uniform!");

            shader->hasUniform("light.quadratic")
                ? shader->setFloat("light.quadratic", light->getQuadratic())
                : Logger::warn("[LightManager::uploadLights] shader has no light.quadratic uniform!");

            // Spotlight
            shader->hasUniform("light.cutOff")
                ? shader->setFloat("light.cutOff", light->getCutOff())
                : Logger::warn("[LightManager::uploadLights] shader has no light.cutOff uniform!");

            shader->hasUniform("light.outerCutOff")
                ? shader->setFloat("light.outerCutOff", light->getOuterCutOff())
                : Logger::warn("[LightManager::uploadLights] shader has no light.outerCutOff uniform!");

            shader->hasUniform("light.type")
                ? shader->setInt("light.type", static_cast<int>(light->getType()))
                : Logger::warn("[LightManager::uploadLights] shader has no light.type uniform!");
        }
        else {
            Logger::warn("Shader doesn't have expected light uniforms");

            // Set default lighting values if no lights are available
            /*shader->hasUniform("light.ambient")
                ? shader->setVec3("light.ambient", glm::vec3(0.2f))
                : Logger::warn("[LightManager::uploadLights] shader has no light.ambient uniform!");*/
        }
    }


    void LightManager::addLightToMap(const std::shared_ptr<Light>& light)
    {
        if (!light) {
            Logger::warn("[LightManager::addLightToMap] light is nullptr, skipping!");
            return;
        }
        auto& sceneObject = light->getSourceObject();
        if (!sceneObject) {
            Logger::warn("[LightManager::addLightToMap] sceneObject is nullptr, skipping!");
            return;
        }
        uint32_t id = sceneObject->getID();
        if (m_mapGetLightWithSceneObjectID.find(id) != m_mapGetLightWithSceneObjectID.end()) {
            Logger::warn("[LightManager::addLightToMap] Light with ID already exists, skipping!");
            return;
        }
		m_mapGetLightWithSceneObjectID[id] = light;
    }


    std::shared_ptr<Light> LightManager::getLightWithSceneObjectID(uint32_t sceneObjectID) const
    {
        auto it = m_mapGetLightWithSceneObjectID.find(sceneObjectID);
        if (it != m_mapGetLightWithSceneObjectID.end()) {
            return it->second;
        } else {
            Logger::warn("[LightManager::getLightWithSceneObjectID] Light with ID not found!");
            return nullptr;
		}
    }
}