#include <Shaders/BasicShader.h>

#include <Shaders/ShaderProgram.h>

#include <graphics/Material/MaterialLib.h>

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include "Scene/SceneObject.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SHADER
{
    BasicShader::BasicShader(const std::shared_ptr<GLShaderProgram>& glShader)
        : m_glProgram(glShader)
    {
        Logger::info("BasicShader created as fallback");
    }

    std::shared_ptr<GLShaderProgram> BasicShader::getGLShaderProgram() const
    {
        return m_glProgram;
    }

    void BasicShader::bind()
    {
        if (m_glProgram) m_glProgram->bind();
        else
            Logger::warn("BasicShader program is can't binding!");
    }

    void BasicShader::setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights)
    {
        for (size_t i = 0; i < lights.size(); ++i) {
            const auto& light = lights[i];
            std::string prefix = "lights[" + std::to_string(i) + "]";

            m_glProgram->setVec3(prefix + ".position",     light->getPosition());
            m_glProgram->setVec3(prefix + ".direction",    light->getDirection());
			m_glProgram->setVec3(prefix + ".ambient",      light->getLightAmbient());
            m_glProgram->setVec3(prefix + ".diffuse",      light->getLightDiffuse());
            m_glProgram->setVec3(prefix + ".specular",     light->getLightSpecular());
            m_glProgram->setFloat(prefix + ".constant",    light->getConstant());
            m_glProgram->setFloat(prefix + ".linear",      light->getLinear());
            m_glProgram->setFloat(prefix + ".quadratic",   light->getQuadratic());
            m_glProgram->setFloat(prefix + ".cutOff",      light->getCutOff());
            m_glProgram->setFloat(prefix + ".outerCutOff", light->getOuterCutOff());
            m_glProgram->setInt(prefix + ".type", static_cast<int>(light->getType()));
        }

        m_glProgram->setInt("numLights", static_cast<int>(lights.size()));
    }

    void BasicShader::setMaterial(const std::shared_ptr<MATERIAL::Material>& mat)
    {
        if (!m_glProgram || !mat) return;

        m_glProgram->setVec3("material.ambient",    mat->m_ambient);
        m_glProgram->setVec3("material.diffuse",    mat->m_diffuse);
        m_glProgram->setVec3("material.specular",   mat->m_specular);
        m_glProgram->setFloat("material.shininess", mat->m_shininess);

        if (mat->m_hasDiffuseTexture == 1) {
            m_glProgram->setInt("material.hasDiffuseTexture", mat->m_hasDiffuseTexture);
            m_glProgram->setInt("diffuseTexture", 0); // texunit
        }
        else {
            m_glProgram->setInt("material.hasDiffuseTexture", 0);
        }

        if (mat->m_hasSpecularTexture == 1) {
            m_glProgram->setInt("material.hasSpecularTexture", mat->m_hasSpecularTexture);
            m_glProgram->setInt("specularTexture", 1); // texunit
        }
        else {
            m_glProgram->setInt("material.hasSpecularTexture", 0);
        }
    }

    void BasicShader::setMatrices(const glm::mat4& model, const glm::mat4& view,
        const glm::mat4& projection, const glm::vec3& cameraPos)
    {
        if (!m_glProgram) return;

        m_glProgram->setUniform("model", model);
        m_glProgram->setUniform("view", view);
        m_glProgram->setUniform("projection", projection);
        m_glProgram->setVec3("viewPos", cameraPos);
    }

    void BasicShader::setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& lightObject)
    {
        if (!lightObject) {
            Logger::warn("[BasicShader::setShaderInterface] lightObject is nullptr!");
            return;
        }
        m_shaderInterface = lightObject;
        DEBUG_PTR(m_shaderInterface);
	}

    std::shared_ptr<SCENE::SceneObject> BasicShader::getShaderInterface() const
    {
        if (!m_shaderInterface) {
            Logger::warn("[BasicShader::getShaderInterface] m_glProgram is nullptr!");
            return nullptr;
        }
		return m_shaderInterface;
    }

}