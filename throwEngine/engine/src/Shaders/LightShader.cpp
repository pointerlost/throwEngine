#include "Shaders/LightShader.h"

#include "graphics/Renderer/RenderData.h"

#include "Shaders/ShaderProgram.h"

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

    void LightShader::setMaterial(const std::shared_ptr<MATERIAL::Material>& mat)
    {
        if (!m_glProgram) return;
        m_glProgram->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.8f));
    }

    void LightShader::setMatrices(const glm::mat4& model, const glm::mat4& view,
        const glm::mat4& projection, const glm::vec3& cameraPos)
    {
        if (!m_glProgram) return;

        m_glProgram->setUniform("model", model);
        m_glProgram->setUniform("view", view);
        m_glProgram->setUniform("projection", projection);
    }
}