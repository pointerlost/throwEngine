#pragma once
#include <iostream>


namespace GLgraphics
{
	class RenderData;
}

// forward declarations
struct GLFWwindow;

namespace ENGINE::UI
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer(std::shared_ptr<GLgraphics::RenderData> renderData, GLFWwindow* window);

		void Init(GLFWwindow* window);
		void BeginFrame();

		void imGuiImplementations();

		void implForLightingPanel();

		void generalMenuForPanel();

		void EndFrame();
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;

		std::shared_ptr<GLgraphics::RenderData> m_renderData;

		bool showDemoWindow = true;
		bool showAnotherWindow = false;
	};
}