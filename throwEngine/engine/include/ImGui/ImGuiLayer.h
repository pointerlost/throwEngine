#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ImGui/ImGuiObjectState.h"


// forward declarations
namespace GLgraphics
{
	class RenderData;
}
struct ImGuiIO;
struct GLFWwindow;
namespace SCENE { class SceneObject; };

namespace ENGINE::UI
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer(std::shared_ptr<GLgraphics::RenderData> renderData, GLFWwindow* window);

		void Init(GLFWwindow* window);
		void BeginFrame();

		void initFont(ImGuiIO& io);

		bool imGuiImplementations(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects);

		void setLightObjectProperties();
		void implForRenderObjects(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects);

		void sceneObjectSelectionPanel(const std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects);

		void lightingPanelWindow();

		void showMaterialProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state);
		void showTransformProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state);

		void showSelectedObjectProperties(std::shared_ptr<SCENE::SceneObject>& object);

		void setSceneObjectMaterial(std::shared_ptr<SCENE::SceneObject>& sceneObject);
		void setSceneObjectTransform(std::shared_ptr<SCENE::SceneObject>& sceneObject);

		void generalMenuForPanel();

		void EndFrame();
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;

		std::shared_ptr<GLgraphics::RenderData> m_renderData;

		bool showDemoWindow = true;
		bool showAnotherWindow = false;

		bool g_RequestShutdown = false;

		std::unordered_map<std::string, UIObjectState> m_objectUIStates;

		bool m_showObjectListWindow   = false;
		bool m_showLightSourceObjects = false;

		std::string m_fontPath;
	};
}
