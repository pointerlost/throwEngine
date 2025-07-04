#include "ImGui/imGuiLayer.h"

#include "ImGui/ImGuiScoped.h"

#include "imgui.h"

#include "imgui_impl_glfw.h"

#include "imgui_impl_opengl3.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace ENGINE::UI
{
	ImGuiLayer::ImGuiLayer(std::shared_ptr<GLgraphics::RenderData> renderData, GLFWwindow* window)
		:m_renderData(renderData), m_Window(window) {}
	
	void ImGuiLayer::Init(GLFWwindow* window)
	{
		m_Window = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsClassic();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 440");	// according to shader version
	}

	void ImGuiLayer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::imGuiImplementations()
	{
		//std::string tableTitle = "Panel Categories";

		//ImGuiScopedTable allPanels(tableTitle.c_str(), 5);

		//if (allPanels)
		//{
		//	// do it some stuff!
		//}

		generalMenuForPanel();

		implForLightingPanel();
	}

	void ImGuiLayer::implForLightingPanel()
	{
		std::string panelTitle = "Lighting Panel";

		// we don't have to call ImGui::End() because we are using destructor of ImGuiScopedWindow for that! (RAII pattern)
		ImGuiScopedWindow lightingPanelWindow(panelTitle.c_str(), nullptr, ImGuiWindowFlags_None);

		auto& io = ImGui::GetIO();

		std::vector<std::shared_ptr<LIGHTING::Light>> lights = m_renderData->getLightManager()->getLightsByVec();

		uint32_t lightSize = lights.size();

		if (lights.empty())
			Logger::warn("lights vector are empty!");

		static int selectedLightIndex = 0;

		ImGui::SliderInt("Change Light Object", &selectedLightIndex, 0, lightSize - 1);

		if (selectedLightIndex >= lightSize) return;

		// current light
		auto& currentLight = lights[selectedLightIndex];

		// we have to create temporary variable to hold the light type
		LIGHTING::LightType currentType = currentLight->getType();
		int typeInt = static_cast<int>(currentType);

		// set light type
		if (ImGui::SliderInt("SetLightType", &typeInt, 0, 3))
			currentLight->setLightType(static_cast<LIGHTING::LightType>(typeInt));
	}

	void ImGuiLayer::generalMenuForPanel()
	{
		ImGuiScopedMainMenuBar mainMenuBar;

		if (mainMenuBar)
		{
			{
				ImGuiScopedMenu engineMenu("ENGINE GUI");
				if (engineMenu)
				{
					ImGui::MenuItem("New");
					ImGui::MenuItem("Open");
					ImGui::MenuItem("Save", "Ctrl+S");
				}
			} // engineMenu destructor calls EndMenu here

			{
				ImGuiScopedMenu otherMenu("OTHER MENU");
				if (otherMenu)
				{
					ImGui::MenuItem("Settings");
					ImGui::MenuItem("Exit");
				}
			}
		}
	}

	void ImGuiLayer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	/********************************** ImGuiScopedWindow *************************************/
}