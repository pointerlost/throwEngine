#pragma once
#include "core/Window.h"
#include "core/File.h"
#include "graphics/Renderer/Renderer.h"
#include "graphics/Camera/CameraController.h"
#include "graphics/GLTransformations/TransformController.h"
#include "Shaders/ShaderManager.h"
#include "Scene/SceneObjectFactory.h"
#include "graphics/Grid/GridSystem.h"
#include "ImGui/ImGuiLayer.h"

static float SCR_WIDTH = 1540.f;
static float SCR_HEIGHT = 860.f;


// Forward Declarations ON

namespace GLgraphics
{
	class Transformations;
	class MeshFactory;
	class RenderData;
}

namespace Input
{
	class Input;
};

namespace LIGHTING
{
	class Light;
	class LightData;
	class LightManager;
}

namespace SCENE
{
	class Scene;
};

namespace TEXTURE { class Textures; };

namespace MATERIAL
{
	class MaterialLibrary;
};

namespace SHADER
{
	class GLShaderProgram;
}

// Forward Declarations OF


namespace core
{

	class Engine {
	public:
		Engine() = default;
		~Engine();

		bool run();
		bool initResources();
		bool initPointerObjects();

	private:

		std::unique_ptr<Window> m_window;

		Input::InputContext dataInputContext;
		
		std::shared_ptr<GLgraphics::MeshData3D> meshData3D;

		std::shared_ptr<GLgraphics::Transformations> transformationManager;
		std::unique_ptr<TRANSFORM_INPUT::TransformInputSystem> transformInput;

		std::shared_ptr<MATERIAL::MaterialLibrary> m_MaterialLibraryPtr;

		std::shared_ptr<CAMERA::Camera> cameraManager;
		std::unique_ptr<CAMERA::CameraInputSystem> cameraInput;

		std::unique_ptr<SCENE::SceneObjectFactory> sceneObjectFactory;

		// Renderer ON
		std::shared_ptr<GLgraphics::RenderData> renderData;

		std::unique_ptr<GLgraphics::Renderer> rendererManager;
		// Renderer OFF

		// Shader Manager ON
		std::shared_ptr<SHADER::ShaderManager> shaderManager;
		// Shader Manager OFF

		// Scene ON
		std::shared_ptr<SCENE::Scene> scene;
		// Scene OFF

		// Texture Manager ON
		std::shared_ptr<TEXTURE::Textures> texture;
		// Texture Manager OFF

		// Lighting ON
		std::shared_ptr<LIGHTING::LightManager> lightManager;
		// Lighting OFF

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		std::unique_ptr<ENGINE::UI::ImGuiLayer> m_imGuiLayer;

		bool initWindow();

		bool initCallBack();

		bool initMaterial();

		bool initGrid();

		bool initMesh();

		bool initShader();

		bool initTexture();

		bool initCamera();

		bool initTransform();

		bool initRender();
		
		bool initImGui();

		bool initScene();

		bool initLighting();

		void OpenGLSetUpResources();

		void OpenGLRenderStuff();

		void glfwRenderEventStuff();

		bool m_RequestShutdown = false;
		void receiveShutdownRequest();
	};
};

