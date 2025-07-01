#include "graphics/Lighting/LightData.h"



namespace LIGHTING
{
	LightData::LightData(const glm::vec3& lightPos)
		: m_pos(lightPos),
		m_SceneCenter(glm::vec3{ 0.0f }),
		m_direction(glm::normalize(m_SceneCenter - m_pos)),
		// SpotLight Stuff
		m_cutOff(glm::cos(glm::radians(12.5))),
		m_outerCutOff(glm::cos(glm::radians(17.5))),
		// Attenuation parameters
		m_constant(1.0),
		m_linear(0.014),
		m_quadratic(0.0007)
	{
		m_lightColor.m_color	= glm::vec3 { 0.98 };
		m_lightColor.m_diffuse  = glm::vec3 { 1.0f, 1.0f, 0.9f };
		m_lightColor.m_specular = glm::vec3 { 1.0f, 1.0f, 1.0f };
	}

	// when sun or an other light objects of changing position, so we have to calculate direction again!
	void LightData::calculateDirection(const glm::vec3& objectPosition)
	{
		m_pos = objectPosition;

		m_direction = glm::normalize(m_SceneCenter - m_pos);
	}

}
