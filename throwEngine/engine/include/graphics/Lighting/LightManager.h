#pragma once
#include <iostream>
#include <vector>
#include <string>
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

	private:
		std::vector<std::shared_ptr<Light>> m_lightsVec;
		static constexpr int MAX_LIGHTS = 4;
	};
}
