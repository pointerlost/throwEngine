#include "ImGui/imGuiLayer.h"

#include "ImGui/ImGuiScoped.h"

#include "imgui_impl_glfw.h"

#include "imgui_impl_opengl3.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include <graphics/GLTransformations/Transformations.h>

#include "Scene/Scene.h"

#include "Scene/SceneObject.h"

#include "Shaders/BasicShader.h"

#include "graphics/Material/MaterialLib.h"

#include <Shaders/ShaderInterface.h>

#include <Config.h>

#include <core/File.h>

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

		// set the ImGui font style and text size
		initFont(io);

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 440");	// according to shader version
	}

	void ImGuiLayer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::initFont(ImGuiIO& io)
	{
		auto& file = core::File::get();

		m_fontPath = std::string(ASSETS_DIR) + "/fonts/Roboto-Italic.ttf";

		if (file.exists(m_fontPath)) {
			io.Fonts->AddFontFromFileTTF(m_fontPath.c_str(), 20.0f);
		}
		else {
			Logger::error("[ImGuiLayer::Init::initFont] Font file not found at path! so Default path assigned!");
			io.Fonts->AddFontDefault();
		}
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

		sceneObjectSelectionPanel(sceneObjects);

		implForRenderObjects(sceneObjects);

		lightingPanelWindow();
		
		//implForLightingPanel();

		return g_RequestShutdown;
	}

	void ImGuiLayer::setLightObjectProperties()
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
		// 0 = Directional, 1 = Point, 2 = Spot
		if (ImGui::SliderInt("SetLightType", &typeInt, 0, 2)) {
			currentType = static_cast<LIGHTING::LightType>(typeInt);
			currentLight->setLightType(currentType);
		}

		auto& lightColor = currentLight->getLightColorProperties();

		ImGui::ColorEdit3("Set Ambient of Light Color",  glm::value_ptr(lightColor.m_ambient));
		ImGui::ColorEdit3("Set Diffuse of Light Color",  glm::value_ptr(lightColor.m_diffuse));
		ImGui::ColorEdit3("Set Specular of Light Color", glm::value_ptr(lightColor.m_specular));

		currentLight->update();
	}

	void ImGuiLayer::implForRenderObjects(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		for (auto& obj : sceneObjects)
		{
			const std::string& name = obj->getObjectName();
			auto& state = m_objectUIStates[name];

			if (!state.isOpen) continue;

			ImGui::SetNextWindowSize(ImVec2(400, 300));

			// Window with close toggle
			ImGuiScopedWindow objectWindow((name + std::string(" | Object Properties")).c_str(), &state.isOpen);
			if (objectWindow)
			{
				showSelectedObjectProperties(obj);
			}
		}
	}

	void ImGuiLayer::sceneObjectSelectionPanel(const std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		if (!m_showObjectListWindow) return;

		uint32_t objectsSize = sceneObjects.size();

		ImGui::SetNextWindowSize(ImVec2(objectsSize * 20, objectsSize * 20), ImGuiCond_Once);

		// ImGuiScopedWindow is a RAII wrapper for ImGui::Begin() and ImGui::End()
		ImGuiScopedWindow objectList("Scene Objects", &m_showObjectListWindow);
		if (!objectList) {
			Logger::warn("[ImGuiLayer::objectSelectionPanel] objectList returning false!");
			return;
		}

		for (auto& obj : sceneObjects)
		{
			const std::string name = obj->getObjectName();
			auto& state = m_objectUIStates[name];

			if (ImGui::Selectable(name.c_str()))
			{
				state.isOpen = !state.isOpen;
			}
		}
	}

	void ImGuiLayer::lightingPanelWindow()
	{
		if (!m_showLightSourceObjects) return;

		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);

		ImGuiScopedWindow lightingPanel("Lighting Panel", &m_showLightSourceObjects);

		if (!lightingPanel) {
			Logger::warn("[ImGuiLayer::lightingPanelWindow] lightingPanel returning false!");
			return;
		}

		setLightObjectProperties();
	}

	void ImGuiLayer::showTransformProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state)
	{
		ImGuiScopedMenu transformProperties("TRANSFORMATIONS");
		if (!transformProperties) return;

		if (ImGui::IsItemHovered())
		{
			state.isTransformOpen = !state.isTransformOpen;
		}

		if (state.isTransformOpen)
		{
			setSceneObjectTransform(object);
		}
	}

	void ImGuiLayer::showMaterialProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state)
	{
		ImGuiScopedMenu materialProperties("MATERIAL");
		if (!materialProperties) return;

		if (ImGui::IsItemHovered())
		{
			state.isMaterialOpen = true;
		}

		if (state.isMaterialOpen)
		{
			setSceneObjectMaterial(object);
		}
	}

	void ImGuiLayer::showSelectedObjectProperties(std::shared_ptr<SCENE::SceneObject>& object)
	{
		auto& state = m_objectUIStates[object->getObjectName()];

		// push style variables to change padding
		ImGuiScopedStyleVar stylePadding(ImGuiStyleVar_FramePadding, ImVec2(15, 12));

		showMaterialProperties(object, state);

		showTransformProperties(object, state);
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

		// ambient
		ImGui::SliderFloat("Set ambient  R value", &material->m_ambient.x,  0.05, 1.0);
		ImGui::SliderFloat("Set ambient  G value", &material->m_ambient.y,  0.05, 1.0);
		ImGui::SliderFloat("Set ambient  B value", &material->m_ambient.z,  0.05, 1.0);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		// diffuse
		ImGui::SliderFloat("Set diffuse  R value", &material->m_diffuse.x,  0.0, 1.0);
		ImGui::SliderFloat("Set diffuse  G value", &material->m_diffuse.y,  0.0, 1.0);
		ImGui::SliderFloat("Set diffuse  B value", &material->m_diffuse.z,  0.0, 1.0);

		// set diffuse color using color picker
		ImGui::ColorEdit3("Set diffuse color", glm::value_ptr(material->m_diffuse),
			ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		// specular
		ImGui::SliderFloat("Set specular R value", &material->m_specular.x, 0.0, 1.0);
		ImGui::SliderFloat("Set specular G value", &material->m_specular.y, 0.0, 1.0);
		ImGui::SliderFloat("Set specular B value", &material->m_specular.z, 0.0, 1.0);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		ImGui::Checkbox("Enable diffuse  texture", &material->m_hasDiffuseTexture);
		ImGui::Checkbox("Enable specular texture", &material->m_hasSpecularTexture);

		sceneObject->setMaterialInstance(material);
	}

	void ImGuiLayer::setSceneObjectTransform(std::shared_ptr<SCENE::SceneObject>& sceneObject)
	{
		ImGui::PushID(sceneObject->getObjectName().c_str()); // Push a unique ID for the object

		auto &transform = sceneObject->getTransform();
		if (!transform) {
			Logger::error("[ImGuiLayer::setSceneObjectTransform] Transform is nullptr!");
			return;
		}

		auto position = transform->getPosition();
		auto angles   = transform->getEulerAngles();
		auto scale	  = transform->getScale();

		ImGui::Text("Position");
		ImGui::SliderFloat("X##Position", &position.x, -100.0, 100.0);
		ImGui::SliderFloat("Y##Position", &position.y, -100.0, 100.0);
		ImGui::SliderFloat("Z##Position", &position.z, -100.0, 100.0);

		ImGui::Separator();
		
		ImGui::Text("Rotation (Euler Angles):");
		ImGui::SliderFloat("X##Euler", &angles.x, -180.0, 180.0);
		ImGui::SliderFloat("Y##Euler", &angles.y, -180.0, 180.0);
		ImGui::SliderFloat("Z##Euler", &angles.z, -180.0, 180.0);

		ImGui::Separator();

		ImGui::Text("Scale");
		ImGui::SliderFloat("X##Scale", &scale.x, 0.01, 10.0);
		ImGui::SliderFloat("Y##Scale", &scale.y, 0.01, 10.0);
		ImGui::SliderFloat("Z##Scale", &scale.z, 0.01, 10.0);

		// Update the transform with the new values
		transform->updateAll(angles, position, scale);

		auto lightSource = sceneObject->getLightSource();
		if (lightSource)
		{
			auto lightData = lightSource->getLightData();

			lightData->setPosition(position);
		
			lightSource->update();
		}

		ImGui::PopID(); // Pop the ID to avoid conflicts with other objects
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

			{
				ImGuiScopedMenu lightingMenu("LIGHTING");
				if (lightingMenu)
				{
					if (ImGui::MenuItem("Change Light Types", nullptr, m_showLightSourceObjects))
						m_showLightSourceObjects = !m_showLightSourceObjects;
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