#include "CallBack/CallBack.h"


namespace CallBack
{
	void initResources(GLFWwindow* window)
	{
		glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
		glfwSetKeyCallback(window, key_CallBack);
		glfwSetCursorPosCallback(window, mouse_CallBack);
		//glfwSetScrollCallback(window, scroll_CallBack);
	}

	void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key < 0 || key >= Input::KEY_MAX) return;

		using namespace Input;

		switch (action)
		{
		case GLFW_PRESS:
			g_IsKeyPressed[key] = true;
			break;

		case GLFW_RELEASE:
			g_IsKeyPressed[key] = false;
			break;

		}
	}

	void mouse_CallBack(GLFWwindow* window, double xpos, double ypos)
	{
		Input::xPosMouse = xpos;
		Input::yPosMouse = ypos;
		Input::updateMouseDelta();
	}
	
}