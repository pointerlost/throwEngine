#include "ImGui/imGuiLayer.h"

#include "ImGui/ImGuiScoped.h"

#include "imgui.h"

#include "imgui_impl_glfw.h"

#include "imgui_impl_opengl3.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include "Scene/Scene.h"

#include "Scene/SceneObject.h"

#include "Shaders/BasicShader.h"

#include "graphics/Material/MaterialLib.h"

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

	bool ImGuiLayer::imGuiImplementations(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		//std::string tableTitle = "Panel Categories";

		//ImGuiScopedTable allPanels(tableTitle.c_str(), 5);

		//if (allPanels)
		//{
		//	// do it some stuff!
		//}

		generalMenuForPanel();

		objectSelectionPanel(sceneObjects);

		implForRenderObjects(sceneObjects);
		
		implForLightingPanel();

		return g_RequestShutdown;
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

	void ImGuiLayer::implForRenderObjects(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		for (auto& obj : sceneObjects)
		{
			const std::string& name = obj->getObjectName();
			auto& state = m_objectUIStates[name];

			if (!state.isOpen) continue;

			// Window with close toggle
			ImGuiScopedWindow objectWindow(name.c_str(), &state.isOpen);
			if (!objectWindow) continue;

			setSceneObjectMaterial(obj);
		}
	}

	void ImGuiLayer::objectSelectionPanel(const std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		if (!m_showObjectListWindow) return;

		ImGuiScopedWindow objectList("Scene Objects", &m_showObjectListWindow);
		if (!objectList) {
			Logger::warn("[ImGuiLayer::objectSelectionPanel] objectList returning false!");
			return;
		}

		for (auto& obj : sceneObjects)
		{
			const std::string& objName = obj->getObjectName();

			// Button to open/close UI panel for object
			if (ImGui::Selectable(objName.c_str()))
			{
				// we have unique names for objects so not needing to check
				auto& state = m_objectUIStates[objName];
				state.isOpen = true; // open the window for this object
			}
		}
	}

	void ImGuiLayer::setSceneObjectMaterial(std::shared_ptr<SCENE::SceneObject>& sceneObject)
	{
		auto object = sceneObject->getShaderInterface();
		auto material = sceneObject->getMaterialInstance();

		if (!object || !material) {
			std::cout << "has sikome!\n";
			return;
		}

		auto objectType = object->getType();

		if (objectType == SHADER::ShaderType::BASIC)
		{
			// ambient
			ImGui::SliderFloat("Set ambient  R value", &material->m_ambient.x,  0.05, 1.0);
			ImGui::SliderFloat("Set ambient  G value", &material->m_ambient.y,  0.05, 1.0);
			ImGui::SliderFloat("Set ambient  B value", &material->m_ambient.z,  0.05, 1.0);

			// diffuse
			ImGui::SliderFloat("Set diffuse  R value", &material->m_diffuse.x,  0.0, 1.0);
			ImGui::SliderFloat("Set diffuse  G value", &material->m_diffuse.y,  0.0, 1.0);
			ImGui::SliderFloat("Set diffuse  B value", &material->m_diffuse.z,  0.0, 1.0);

			// specular
			ImGui::SliderFloat("Set specular R value", &material->m_specular.x, 0.0, 1.0);
			ImGui::SliderFloat("Set specular G value", &material->m_specular.y, 0.0, 1.0);
			ImGui::SliderFloat("Set specular B value", &material->m_specular.z, 0.0, 1.0);

			ImGui::Checkbox("Enable diffuse  texture", &material->m_hasDiffuseTexture);
			ImGui::Checkbox("Enable specular texture", &material->m_hasSpecularTexture);
		}
		else if (objectType == SHADER::ShaderType::LIGHT)
		{
		}
	}

	void ImGuiLayer::generalMenuForPanel()
	{
		static bool showNewMenu = false;
		
		ImGuiScopedMainMenuBar mainMenuBar;

		if (mainMenuBar)
		{
			{
				ImGuiScopedMenu engineMenu("ENGINE GUI");
				if (engineMenu)
				{
					if (ImGui::MenuItem("New"))
						showNewMenu = true;

					ImGui::MenuItem("Save", "Ctrl+S");
				}
			} // engineMenu destructor calls EndMenu here

			{
				ImGuiScopedMenu sceneObjectsMenu("SCENE");
				if (sceneObjectsMenu)
				{
					// SceneObjects GUI
					if (ImGui::MenuItem("SceneObjects", nullptr, m_showObjectListWindow))
						m_showObjectListWindow = !m_showObjectListWindow;

					// Engine Shutdown
					if (ImGui::MenuItem("Exit"))
						g_RequestShutdown = true;
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