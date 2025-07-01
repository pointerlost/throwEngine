#pragma once
#include "core/Window.h"
#include "Input/Input.h"


namespace CallBack
{
	void initResources(GLFWwindow* window);

	void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	void key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_CallBack(GLFWwindow* window, double xpos, double ypos);

	//void scroll_CallBack(GLFWwindow* window, double xoffset, double yoffset);
}