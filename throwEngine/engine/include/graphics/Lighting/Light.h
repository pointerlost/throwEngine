#pragma once
#include <iostream>
#include <memory>
#include <graphics/Lighting/LightData.h>

namespace SCENE { class SceneObject; }

namespace LIGHTING
{

	enum class LightType
	{
		Point		= 0,
		Directional = 1,
		Spot		= 2
	};

	struct LightColor
	{
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
	};

	class Light
	{
	public:
		Light(std::shared_ptr<SCENE::SceneObject> sceneObject, std::shared_ptr<LightData> lightData);

		void update();

		std::shared_ptr<SCENE::SceneObject>& getSourceObject() { return m_SceneObject; };

		void setLightType(LightType type) { m_type = type; };
		LightType getType() const { return m_type; };

		void setLightData(std::shared_ptr<LightData> lightData) { m_lightData = lightData; };
		std::shared_ptr<LightData> getLightData() const { return m_lightData; };

		const glm::vec3& getDirection()     const { return m_lightData->getDirection(); };
		const glm::vec3& getPosition()      const { return m_lightData->getPosition();  };
		
		// spotlight on
		void setCutOff(float cutOff) { m_lightData->setCutOff(cutOff); }
		float getCutOff() const { return m_lightData->getCutOff(); }

		void setOuterCutOff(float outerCutOff) { m_lightData->setOuterCutOff(outerCutOff); }
		float getOuterCutOff() const { return m_lightData->getOuterCutOff(); }

		void setConstant(float constant) { m_lightData->setConstant(constant); }
		float getConstant() const { return m_lightData->getConstant(); }

		void setLinear(float linear) { m_lightData->setLinear(linear); }
		float getLinear() const { return m_lightData->getLinear(); }

		void setQuadratic(float quadratic) { m_lightData->setQuadratic(quadratic); }
		float getQuadratic() const { return m_lightData->getQuadratic(); }
		// spotlight off

		// Light Material
		void setLightAmbient(const glm::vec3& ambient) { m_lightObjectColor.m_ambient = ambient; };
		void setLightDiffuse(const glm::vec3& diffuse) { m_lightObjectColor.m_diffuse = diffuse; };
		void setLightSpecular(const glm::vec3& specular) { m_lightObjectColor.m_specular = specular; };
		const glm::vec3& getLightAmbient()   const  { return m_lightObjectColor.m_ambient;   };
		const glm::vec3& getLightDiffuse()   const  { return m_lightObjectColor.m_diffuse;   };
		const glm::vec3& getLightSpecular()  const  { return m_lightObjectColor.m_specular;  };

		LightColor& getLightColorProperties() { return m_lightObjectColor; };

		bool isValid() const { return m_SceneObject != nullptr && m_lightData != nullptr; };

		void setSceneObjectID(uint32_t id) { m_sceneObjectID = id; };
		uint32_t getSceneObjectID() const { return m_sceneObjectID; };

	private:
		std::shared_ptr<SCENE::SceneObject> m_SceneObject;

		uint32_t m_sceneObjectID = 0;

		std::shared_ptr<LightData> m_lightData;

		LightType m_type;

		LightColor m_lightObjectColor;
	};
}