#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


namespace LIGHTING
{

	struct LightColor
	{
		glm::vec3 m_color;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
	};

	class LightData
	{
	public:
		LightData(const glm::vec3& lightPos);

		glm::vec3& getLightObjectColor() { return m_lightColor.m_color; };

		// LightColor
		void setDiffuse(const glm::vec3& diffuse)   { m_lightColor.m_diffuse   = diffuse;   };
		void setSpecular(const glm::vec3& specular) { m_lightColor.m_specular  = specular;  };

		const glm::vec3& getDiffuse()   const { return m_lightColor.m_diffuse;   };
		const glm::vec3& getSpecular()  const { return m_lightColor.m_specular;  };

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
		float m_cutOff;
		float m_outerCutOff;
		float m_constant;
		float m_linear;
		float m_quadratic;

		LightColor m_lightColor;
	};
}