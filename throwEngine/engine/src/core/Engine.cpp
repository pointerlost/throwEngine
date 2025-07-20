#include "core/Engine.h"

#include "Config.h"

#include <core/Debug.h>

#include "CallBack/CallBack.h"

#include "Shaders/ShaderManager.h"

#include "graphics/GLTransformations/Transformations.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Textures/Textures.h"

#include "graphics/Material/MaterialLib.h"

#include "Scene/Scene.h"

#include "Scene/SceneObject.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightData.h"

#include "graphics/Lighting/LightManager.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "graphics/Camera/Camera.h"

#include <Input/Input.h>

#include "core/Logger.h"
// preprocessors
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace core {

	Engine::~Engine()
	{
		m_imGuiLayer->Shutdown();
	}

	bool Engine::run()
	{
		OpenGLSetUpResources();

		auto window = m_window->getGLFWwindow();

		if (!window)
			Logger::error("[Engine::run] GLFWwindow returning nullptr!");

		// engine life loop
		while (!glfwWindowShouldClose(window) && !m_RequestShutdown)
		{
			OpenGLRenderStuff();

			cameraInput->processInput(window);
			
			Input::update();

			m_imGuiLayer->BeginFrame();

			rendererManager->draw(scene, cameraManager->getViewMatrix(), cameraManager->getProjectionMatrix());

			m_RequestShutdown = m_imGuiLayer->imGuiImplementations(scene->getSceneObjectVector());

			m_imGuiLayer->EndFrame();

			glfwRenderEventStuff();
		}

		// test
		//cameraManager->updateCamera();

		std::cout << "i hope so!" << "\n";
		return true;
	}

	bool Engine::initResources()
	{
		if (!m_MaterialLibraryPtr->loadMaterialFromJSON(MATERIALS_JSON_PATH, texture)) {
			Logger::error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		if (!shaderManager->loadAllShaders()) {
			Logger::error("[Engine::initResources] loadAllShaders failed!");
			return false;
		}

		if (!scene->SetUpResources(m_MaterialLibraryPtr)) {
			Logger::error("[Engine::initResources] scene->SetUpResources failed!");
			return false;
		}

		m_imGuiLayer->Init(m_window->getGLFWwindow());

		scene->initGrid(renderData);

		Logger::info("Engine SetUpResources successfull!");
		return true;
	}

	bool Engine::initPointerObjects()
	{
		try {
			if (!initWindow())
				throw std::runtime_error("initWindow failed!");
			
			if (!initCallBack())
				throw std::runtime_error("initCallBack failed!");
			
			if (!initShader())
				throw std::runtime_error("initShader failed!");

			if (!initMesh())
				throw std::runtime_error("initMesh failed!");

			if (!initTexture())
				throw std::runtime_error("initTexture failed!");
		
			if (!initMaterial())
				throw std::runtime_error("initMaterial failed!");
			
			if (!initCamera())
				throw std::runtime_error("initCamera failed!");
			
			if (!initTransform())
				throw std::runtime_error("initTransform failed!");
			
			if (!initRender())
				throw std::runtime_error("initRender failed!");

			if (!initImGui())
				throw std::runtime_error("initImGui failed!");
			
			if (!initScene())
				throw std::runtime_error("initScene failed!");

			if (!initGrid())
				throw std::runtime_error("initGrid failed!");
			
			if (!initLighting())
				throw std::runtime_error("initLighting failed!");
		}
		catch (const std::exception& e) {
			Logger::error("Engine::initResources FAILED! Error: " + std::string(e.what()));
			return false;
		}

		Logger::info("Engine initPointerObjects successfull!");
		return true;
	}

	bool Engine::initWindow()
	{
		// Window Manager
		m_window = std::make_unique<Window>();
		
		DEBUG_PTR(m_window);

		m_window->initResources();

		return true;
	}

	bool Engine::initCallBack()
	{
		CallBack::initResources(m_window->getGLFWwindow());
		
		return true;
	}

	bool Engine::initShader()
	{
		shaderManager = std::make_unique<SHADER::ShaderManager>();

		if (!shaderManager) {
			Logger::warn("[Engine::initShader] shaderManager object is nullptr");
			return false;
		}

		DEBUG_PTR(shaderManager);

		return true;
	}

	bool Engine::initMesh()
	{
		sceneObjectFactory = std::make_unique<SCENE::SceneObjectFactory>();

		meshData3D = sceneObjectFactory->getMeshData();

		if (!sceneObjectFactory || !sceneObjectFactory->getMeshData()) {
			Logger::error("Failed to initialize SceneObjectFactory");
			return false;
		}
		
		DEBUG_PTR(sceneObjectFactory);

		return true;
	}

	bool Engine::initTexture()
	{
		texture = std::make_shared<TEXTURE::Textures>();

		if (!texture) {
			Logger::warn("[Engine::initTexture] texture is nullptr!");
			return false;
		}

		DEBUG_PTR(texture);

		return true;
	}

	bool Engine::initMaterial()
	{
		using namespace MATERIAL;

		m_MaterialLibraryPtr = std::make_shared<MaterialLibrary>();

		if (!m_MaterialLibraryPtr) {
			Logger::warn("[Engine::initMaterial] m_MaterialLibraryPtr is nullptr!");
			return false;
		}

		DEBUG_PTR(m_MaterialLibraryPtr);

		return true;
	}

	bool Engine::initCamera()
	{
		cameraManager = std::make_shared<CAMERA::Camera>();

		if (!cameraManager) {
			Logger::error("[Engine::initCamera] cameraManager is nullptr!");
			return false;
		}

		DEBUG_PTR(cameraManager);

		cameraInput = std::make_unique<CAMERA::CameraInputSystem>(cameraManager.get(), dataInputContext);

		if (!cameraInput) {
			Logger::warn("[Engine::initCamera] cameraInput is nullptr!");
			return false;
		}

		DEBUG_PTR(cameraInput);

		return true;
	}

	bool Engine::initTransform()
	{
		transformationManager = std::make_shared<GLgraphics::Transformations>();

		if (!transformationManager) {
			Logger::warn("[Engine::initTransform] transformationManager is nullptr!");
			return false;
		}

		DEBUG_PTR(transformationManager);

		transformInput = std::make_unique<TRANSFORM_INPUT::TransformInputSystem>(transformationManager.get(), dataInputContext);

		if (!cameraInput) {
			Logger::warn("[Engine::initTransform] transformInput is nullptr!");
			return false;
		}

		DEBUG_PTR(transformInput);
		
		return true;
	}

	bool Engine::initRender()
	{
		renderData = std::make_shared<GLgraphics::RenderData>(shaderManager, cameraManager, texture, m_MaterialLibraryPtr);

		if (!renderData) {
			Logger::warn("[Engine::initRender] renderData is nullptr!");
			return false;
		}

		DEBUG_PTR(renderData);

		rendererManager = std::make_unique<GLgraphics::Renderer>(renderData);

		if (!rendererManager) {
			Logger::warn("[Engine::initRender] rendererManager is nullptr!");
			return false;
		}

		DEBUG_PTR(rendererManager);

		return true;
	}

	bool Engine::initImGui()
	{
		m_imGuiLayer = std::make_unique<ENGINE::UI::ImGuiLayer>(renderData, m_window->getGLFWwindow());

		if (!m_imGuiLayer) {
			Logger::warn("[Engine::initImGui] m_imGuiLayer object is nullptr");
			return false;
		}

		return true;
	}

	bool Engine::initScene()
	{
		scene = std::make_shared<SCENE::Scene>(meshData3D, dataInputContext);

		if (!scene) {
			Logger::warn("[Engine::initScene] scene is nullptr!");
			return false;
		}

		DEBUG_PTR(scene);

		scene->setShaderManager(renderData->getShaderManager());

		scene->setSceneObjectFactoryPointer(sceneObjectFactory.get());

		return true;
	}

	bool Engine::initGrid()
	{
		// Init Grid
		m_gridSystem = std::make_unique<GRID::GridSystem>();

		if (!m_gridSystem) {
			Logger::warn("[Engine::initGrid] m_gridSystem returning nullptr");
			return false;
		}

		if (!m_gridSystem->initialize(cameraManager)) {
			Logger::warn("[Engine::initGrid] m_gridSystem initializing failed!");
			return false;
		}

		renderData->setGridRenderer(m_gridSystem->getRenderer());

		scene->setOwnershipGridSystemToScene(m_gridSystem);

		// before we use grid renderer we have to set a shader interface(like grid shader) to gridRenderer!
		// so create a shader interface for grid shader, and send to the gridRenderer, because needed for drawing!

		return true;
	}

	bool Engine::initLighting()
	{
		lightManager = std::make_shared<LIGHTING::LightManager>();

		if (!lightManager) {
			Logger::warn("[Engine::initLighting] lightManager is nullptr!");
		}

		DEBUG_PTR(lightManager);
		
		renderData->setLightManager(lightManager);
		scene->setLightManager(lightManager);

		return true;
	}

	void Engine::OpenGLSetUpResources()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Engine::OpenGLRenderStuff()
	{
		// Screen Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Engine::glfwRenderEventStuff()
	{
		// GLFW stuff
		glfwPollEvents();
		glfwSwapBuffers(m_window->getGLFWwindow());
	}
}