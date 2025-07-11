#include "graphics/Lighting/Light.h"

#include "Scene/SceneObject.h"
#include "graphics/GLTransformations/Transformations.h"

namespace LIGHTING
{

	Light::Light(std::shared_ptr<SCENE::SceneObject> sceneObject, std::shared_ptr<LightData> lightData)
		: m_SceneObject(sceneObject), m_lightData(lightData)
	{
		// Setup context
		// set light position to sun object! like a light source!
		m_lightData->setPosition(m_SceneObject->getTransform()->getPosition());

		// Material Initialization
		m_lightObjectColor.m_ambient   = glm::vec3{ 0.24725f,  0.1995f,   0.0745f   };
		m_lightObjectColor.m_diffuse   = glm::vec3{ 0.75164f,  0.60648f,  0.22648f  };
		m_lightObjectColor.m_specular  = glm::vec3{ 0.628281f, 0.555802f, 0.366065f };
	}

	void Light::update()
	{
		m_lightData->setPosition(m_SceneObject->getTransform()->getPosition());
		m_lightData->calculateDirection(m_SceneObject->getTransform()->getPosition());
	}
}