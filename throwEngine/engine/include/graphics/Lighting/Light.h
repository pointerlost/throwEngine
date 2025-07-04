#pragma once
#include <iostream>

#include <graphics/Lighting/LightData.h>

namespace SCENE { class SceneObject; }

namespace LIGHTING
{

	enum class LightType
	{
		Point,
		Directional,
		SpotLight
	};

	struct Material
	{
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;

		glm::vec3 m_color;
	};

	class Light
	{
	public:
		Light(std::shared_ptr<SCENE::SceneObject> sceneObject, std::shared_ptr<LightData> lightData);

		void update();

		std::shared_ptr<SCENE::SceneObject>& getSourceObject() { return m_SceneObject; };

		glm::vec3& getLightObjectColor() const { return m_lightData->getLightObjectColor(); };

		void setLightType(LightType type) { m_type = type; };
		LightType getType() const { return m_type; };

		// Light Color
		const glm::vec3& getLightDiffuse()  const { return m_lightData->getDiffuse();   };
		const glm::vec3& getLightSpecular() const { return m_lightData->getSpecular();  };
		
		const glm::vec3& getDirection()     const { return m_lightData->getDirection(); };
		const glm::vec3& getPosition()      const { return m_lightData->getPosition();  };
		
		// spotlight
		float getCutOff()	   const { return m_lightData->getCutOff();		 };
		float getOuterCutOff() const { return m_lightData->getOuterCutOff(); };
		float getConstant()    const { return m_lightData->getConstant();    };
		float getLinear()	   const { return m_lightData->getLinear();		 };
		float getQuadratic()   const { return m_lightData->getQuadratic();   };

		// Light Material
		const glm::vec3& getMaterialAmbient()   const  { return m_lightObjectMaterial.m_ambient;   };
		const glm::vec3& getMaterialDiffuse()   const  { return m_lightObjectMaterial.m_diffuse;   };
		const glm::vec3& getMaterialSpecular()  const  { return m_lightObjectMaterial.m_specular;  };
		const	   float getMaterialShininess() const  { return m_lightObjectMaterial.m_shininess; };

		const Material& getObjectMaterial() const { return m_lightObjectMaterial; };

	private:
		std::shared_ptr<SCENE::SceneObject> m_SceneObject;
		std::shared_ptr<LightData> m_lightData;

		LightType m_type;

		Material m_lightObjectMaterial;
	};
}