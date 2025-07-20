#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <unordered_map>


namespace LIGHTING
{

	class LightData
	{
	public:
		LightData(const glm::vec3& lightPos);

		void calculateDirection(const glm::vec3& objectPosition);
		void setPosition(const glm::vec3& pos)  { m_pos = pos; };

		const glm::vec3& getDirection() const { return m_direction; };
		const glm::vec3& getPosition()  const { return m_pos;		};
		
		void setCutOff(float cutOff) { m_cutOff = cutOff; };
		float getCutOff()	   const { return m_cutOff;      };

		void setOuterCutOff(float outerCutOff) { m_outerCutOff = outerCutOff; };
		float getOuterCutOff() const { return m_outerCutOff; };

		void setConstant(float constant) { m_constant = constant; };
		float getConstant()    const { return m_constant;    };

		void setLinear(float linear) { m_linear = linear; };
		float getLinear()	   const { return m_linear;		 };

		void setQuadratic(float quadratic) { m_quadratic = quadratic; };
		float getQuadratic()   const { return m_quadratic;   };

	private:
		glm::vec3 m_pos;
		glm::vec3 m_direction;
		// scene center reference point
		glm::vec3 m_SceneCenter;

		// spotlight soft edge section
		float m_cutOff		= (glm::cos(glm::radians(12.5)));
		float m_outerCutOff = (glm::cos(glm::radians(17.5)));

		// attenuation parameters
		float m_constant  = 1.0f;
		float m_linear	  = 0.014f;
		float m_quadratic = 0.0007f;
	};
}