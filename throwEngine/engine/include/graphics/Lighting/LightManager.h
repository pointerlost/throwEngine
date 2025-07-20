#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "graphics/Lighting/Light.h"

namespace LIGHTING
{
	class LightData;
}

namespace SHADER
{
	class GLShaderProgram;
}

namespace LIGHTING
{

	class LightManager
	{
	public:
		LightManager() = default;

		void addLightToVec(const std::shared_ptr<Light>& light) { m_lightsVec.push_back(light); };

		const std::vector<std::shared_ptr<Light>>& getLightsByVec() const { return m_lightsVec; };

		void uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader) const;

		void addLightToMap(const std::shared_ptr<Light>& light);
		std::shared_ptr<Light> getLightWithSceneObjectID(uint32_t sceneObjectID) const;

	private:
		std::vector<std::shared_ptr<Light>> m_lightsVec;

		std::unordered_map<uint32_t, std::shared_ptr<Light>> m_mapGetLightWithSceneObjectID;

		static constexpr int MAX_LIGHTS = 6;
	};
}
