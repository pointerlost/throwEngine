#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace GLgraphics
{
	// Forward declaration
	class Shader;
	class Mesh;

	class Transformations
	{
	public:
		Transformations() = default;
		~Transformations() = default;

		void setPosition(const glm::vec3& pos);
		void setRotation(float angleInDegrees, const glm::vec3& axis);
		glm::vec3 getRotationAxis() const { return rotationAxis; };

		void setRotationAngle(float angleInDegrees) { rotationAngle = angleInDegrees; };
		float getRotationAngle() const { return rotationAngle; };

		void setScale(const glm::vec3& scaleVec);
		glm::vec3 getScale() const { return scale; };

		void addRotation(float angleInDegrees, const glm::vec3& axis);

		glm::vec3 getPosition() const { return position; };

		void updateAll(const glm::vec3& angles, const glm::vec3& pos, const glm::vec3& scaleVec);

		void resetAll();

		glm::mat4 getModelMatrix() const;

		void setEulerAngles(const glm::vec3& angles) { eulerAngles = angles; };
		glm::vec3 getEulerAngles() const { return eulerAngles; }

		void setPitchYawRoll(float pitch, float yaw, float roll) { eulerAngles = glm::vec3(pitch, yaw, roll); };

	private:

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		float rotationAngle = 45.0f; // degrees
		glm::vec3 scale = glm::vec3(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::vec3 eulerAngles = glm::vec3(0.0f);
	};
}

