#include "Shaders/LightShader.h"

#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

#include <graphics/Lighting/Light.h>

#include <graphics/Material/MaterialLib.h>

#include "scene/SceneObject.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SHADER
{
    LightShader::LightShader(const std::shared_ptr<GLShaderProgram>& glShader)
        : m_glProgram(glShader)
    {
    }

    std::shared_ptr<GLShaderProgram> LightShader::getGLShaderProgram() const
    {
        return m_glProgram;
    }

    void LightShader::bind()
    {
        if (m_glProgram) m_glProgram->bind();
        else
            Logger::warn("BasicShader program is can't binding!");
    }

    void LightShader::setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights)
    {
        if (!m_glProgram) {
			Logger::warn("[LightShader::setLights] m_glProgram is nullptr!");
            return;
        }
        if (lights.empty()) {
            Logger::warn("[LightShader::setLights] lights vector is empty!");
            return;
        }
		if (!m_lightShaderInterface) {
			Logger::warn("[LightShader::setLights] m_lightShaderInterface is nullptr!");
            return;
		}

        for (auto& light : lights) {
            if (!light) {
				Logger::warn("[LightShader::setLights] light is nullptr, skipping!");
                continue;
            }
            if (light.get() == m_lightShaderInterface->getLightSource().get()) {

                auto& lightProp = light->getLightColorProperties();

                // set light type
                m_glProgram->setInt("lightType", static_cast<int>(light->getType()));

				// light color properties
                m_glProgram->setVec3("lightAmbient",  lightProp.m_ambient  );
                m_glProgram->setVec3("lightDiffuse",  lightProp.m_diffuse  );
                m_glProgram->setVec3("lightSpecular", lightProp.m_specular );

				// light position and direction
				m_glProgram->setVec3("lightPos", light->getPosition());
				m_glProgram->setVec3("lightDir", light->getDirection());
            }
        }
	}

    void LightShader::setMatrices(const glm::mat4& model, const glm::mat4& view,
        const glm::mat4& projection, const glm::vec3& cameraPos)
    {
        if (!m_glProgram) {
			Logger::warn("[LightShader::setMatrices] m_glProgram is nullptr!");
            return;
        }

        m_glProgram->setUniform("model",      model       );
        m_glProgram->setUniform("view",       view        );
        m_glProgram->setUniform("projection", projection  );
        m_glProgram->setVec3   ("viewPos",    cameraPos   );
    }

    void LightShader::setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& sceneVisualObject)
    {
        if (!sceneVisualObject) {
            Logger::warn("[LightShader::setShaderInterface] lightObject is nullptr!");
            return;
        }
        m_lightShaderInterface = sceneVisualObject;
        DEBUG_PTR(m_lightShaderInterface);
	}

    std::shared_ptr<SCENE::SceneObject> LightShader::getShaderInterface() const
    {
        if (!m_lightShaderInterface) {
            Logger::warn("[LightShader::getShaderInterface] m_lightShaderInterface is nullptr!");
            return nullptr;
        }
		return m_lightShaderInterface;
    }

}