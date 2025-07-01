#include "Input/InputComponent.h"

#include "graphics/GLTransformations/Transformations.h"

#include "Scene/Scene.h"

#include "Input/Input.h"

#include "Scene/SceneObject.h"

#include "graphics/Lighting/LightManager.h"
#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace InputComponent
{

	CubeInputComponent::CubeInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)
		: m_transformation(transform), m_dataContext(context)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void CubeInputComponent::processInput(SCENE::Scene &scene)
	{
		std::vector<std::shared_ptr<SCENE::SceneObject>> sceneObjects = scene.getSceneObjectVector();

		for (const auto& obj : sceneObjects) {

			if (obj->getObjectName() == "cube_0")
			{
				m_transformation = obj->getTransform();

				if (Input::g_IsKeyHeld[GLFW_KEY_DOWN]) {
					m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, -3.0f, 0.0f });
					//std::cout << "'DOWN' pressed!" << "\n";
				}
				if (Input::g_IsKeyHeld[GLFW_KEY_UP]) {
					m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, 3.0f, 0.0f });
					//std::cout << "'UP' pressed!" << "\n";
				}
				if (Input::g_IsKeyHeld[GLFW_KEY_LEFT]) {
					m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ -3.0f, 0.0f, 0.0f });
					//std::cout << "'LEFT' pressed!" << "\n";
				}
				if (Input::g_IsKeyHeld[GLFW_KEY_RIGHT]) {
					m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 3.0f, 0.0f, 0.0f });
					//std::cout << "'RIGHT' pressed!" << "\n";
				}
				if (Input::g_IsKeyHeld[GLFW_KEY_ENTER]) {
					m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, 0.0f, 3.0f });
					//std::cout << "'ENTER' pressed!" << "\n";
					//std::cout << dataContext.m_angle++ << "\n";
				}

				break;
			}
		}
	}

	void CubeInputComponent::setUpInputContext()
	{
		/* CALCULATION back later
		dataContext.m_currentTime = glfwGetTime();
		dataContext.m_delta = dataContext.m_currentTime - dataContext.m_lastTime;
		dataContext.m_angle = dataContext.m_delta * dataContext.m_rotationSpeed;
		dataContext.m_lastTime = dataContext.m_currentTime;
		*/

		m_dataContext.m_rotationSpeed = 1.0f;
		m_dataContext.m_delta = -90.0f;
		m_dataContext.m_angle = 0.5f;
	}


	CircleInputComponent::CircleInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)
		: m_transformation(transform), m_dataContext(context)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void CircleInputComponent::setUpInputContext()
	{
		m_dataContext.m_rotationSpeed = 0.3f;
		m_dataContext.m_delta = -90.0f;
		m_dataContext.m_angle = 0.5f;
	}

	/*****************************************************************************************************/

	SunInputComponent::SunInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)

		: m_transformation(transform), m_dataContext(context)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void SunInputComponent::processInput(SCENE::Scene& scene)
	{
		std::vector<std::shared_ptr<SCENE::SceneObject>> sceneObjects = scene.getSceneObjectVector();

		auto& lightManager = scene.getLightManager();

		if (!lightManager) {
			Logger::warn("[SunInputComponent] lightManager returning nullptr.");
			return;
		}

		std::vector<std::shared_ptr<LIGHTING::Light>> lightVec = lightManager->getLights();
		if (lightVec.empty()) {
			Logger::warn("[SunInputComponent] lightVec is empty.");
			return;
		}

		m_inputSelectors.resize(lightVec.size(), false);
		m_inputSelectors[m_activeLightComponentIdx] = true;

		static bool isRotating = false;

		if (Input::g_IsKeyHeld[GLFW_KEY_3]) {
			isRotating = !isRotating;
		}

		if (Input::g_IsKeyPressed[GLFW_KEY_F10] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == 0) {
				m_activeLightComponentIdx = lightVec.size() - 1;
			}
			else {
				--m_activeLightComponentIdx;
			}
		}

		if (Input::g_IsKeyPressed[GLFW_KEY_F11] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == lightVec.size() - 1) {
				m_activeLightComponentIdx = 0;
			}
			else {
				++m_activeLightComponentIdx;
			}
		}

		glm::vec3 moveOffset{ 0.0 };
		const float moveSpeed = 2.0 * m_dataContext.m_delta;

		for (const auto& obj : sceneObjects)
		{
			for (int i = 0; i < lightVec.size(); ++i)
			{
				if ( obj->getObjectName() == "sphere_" + std::to_string(i) && m_inputSelectors[i] )
				{
					m_transformation = obj->getTransform();
					glm::vec3 objectPos = m_transformation->getPosition();

					if (isRotating)
					{
						m_dataContext.m_angle += m_dataContext.m_rotationSpeed * m_dataContext.m_delta;

						glm::vec3 newPos{};

						newPos.x = m_dataContext.m_radius * cos(m_dataContext.m_angle);
						newPos.z = m_dataContext.m_radius * sin(m_dataContext.m_angle);

						newPos.y = 10 + 60 * sin(m_dataContext.m_angle);

						m_transformation->setPosition(newPos);
						lightVec[i]->update();
					}
					else {
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_8]) moveOffset.z -= moveSpeed;
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_2]) moveOffset.z += moveSpeed;
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_4]) moveOffset.x -= moveSpeed;
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_6]) moveOffset.x += moveSpeed;
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_7]) moveOffset.y += moveSpeed;
						if (Input::g_IsKeyHeld[GLFW_KEY_KP_1]) moveOffset.y -= moveSpeed;

						if (glm::length(moveOffset) > 0.0) {
							glm::vec3 newPos = objectPos + moveOffset;
							m_transformation->setPosition(newPos);
							lightVec[i]->update();
						}
					}

					break;
				}
			}
		}
	}

	void SunInputComponent::setUpInputContext()
	{
		m_dataContext.m_rotationSpeed = 0.5f;
		m_dataContext.m_delta = 0.016f;
		m_dataContext.m_angle = 0.0f;
		m_dataContext.m_radius = 45.0f;
	}

}

