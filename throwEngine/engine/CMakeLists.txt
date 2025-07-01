cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/engine/src/ShadersGLSL")
message(STATUS "Shader directory: ${SHADERS_DIR}")

if(NOT EXISTS ${SHADERS_DIR})
    message(FATAL_ERROR "Shaders directory not found at: ${SHADERS_DIR}")
endif()

set(SHADERS_CONFIG_PATH "${SHADERS_DIR}/configs/shaders.json")
set(GLSL_DIR "${SHADERS_DIR}/GLSL")
set(CONFIGS_DIR "${SHADERS_DIR}/configs")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/GLSL)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/configs)

# Copy shader files to build directory
file(GLOB SHADER_FILES "${SHADERS_DIR}/GLSL/*")
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/GLSL)

# Copy config files
file(COPY "${SHADERS_DIR}/configs/shaders.json" 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/configs)

if(EXISTS ${GLSL_DIR})
    file(GLOB GLSL_FILES "${GLSL_DIR}/*")
    file(COPY ${GLSL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/GLSL)
endif()

if(EXISTS ${CONFIGS_DIR})
    file(GLOB CONFIG_FILES "${CONFIGS_DIR}/*")
    file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/configs)
endif()

# Set your assets directory (adjust path as needed)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/engine/assets")

# Make sure this is before configure_file()
if(NOT EXISTS "${ASSETS_DIR}/materialJSON/materials.json")
    message(FATAL_ERROR "Materials JSON file not found at: ${ASSETS_DIR}/materialJSON/materials.json")
endif()

# Create assets directory in build folder
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets")

add_library(engine STATIC

	# core .cpp
	src/core/Engine.cpp
	src/core/Window.cpp
	src/core/File.cpp

	src/CallBack/CallBack.cpp

	src/Input/Input.cpp
	src/Input/InputContext.cpp
	src/Input/InputComponent.cpp
	
	# graphic .cpp
	src/graphics/Mesh/GLMesh3D.cpp
	src/graphics/Mesh/GLMeshFactory.cpp
	src/graphics/Mesh/GLMeshData3D.cpp
	src/graphics/Mesh/GLMeshTriangle3D.cpp
	src/graphics/Mesh/GLMeshSquare3D.cpp
	src/graphics/Mesh/GLMeshCube3D.cpp
	src/graphics/Mesh/GLMeshCircle3D.cpp
	src/graphics/Mesh/GLMeshSphere3D.cpp

	# Shader manager
	src/Shaders/ShaderProgram.cpp
	src/Shaders/ShaderManager.cpp
	src/Shaders/BasicShader.cpp

	# # # grid shader  # # #
	src/Shaders/GridShader.cpp
	# # # light shader # # #
	src/Shaders/LightShader.cpp

	# Camera
	src/graphics/Camera/Camera.cpp
	src/graphics/Camera/CameraController.cpp

	# Transformations
	src/graphics/GLTransformations/Transformations.cpp
	src/graphics/GLTransformations/TransformController.cpp

	# Grid
	src/graphics/Grid/GridData.cpp
	src/graphics/Grid/GridRenderer.cpp
	src/graphics/Grid/GridSystem.cpp

	# Scene system
	src/Scene/Scene.cpp
	src/Scene/SceneObject.cpp
	src/Scene/SceneObjectFactory.cpp
	
	# graphic -> render system
	src/graphics/Renderer/RenderData.cpp
	src/graphics/Renderer/Renderer.cpp

	# graphic -> Lighting system
	src/graphics/Lighting/Light.cpp
	src/graphics/Lighting/LightData.cpp
	src/graphics/Lighting/LightManager.cpp

	# graphic -> Textures
	src/graphics/Textures/Textures.cpp
	src/graphics/Textures/stb_image.cpp

	# graphic -> Material
	src/graphics/Material/MaterialLib.cpp
	
	# core .hpp
	include/core/Engine.h
	include/core/Window.h
	include/core/File.h
	include/core/Logger.h

	# callback stuff
	include/CallBack/CallBack.h

	# Input stuff
	include/Input/Input.h
	include/Input/InputContext.h
	include/Input/InputComponent.h

	# graphic .hpp
	include/graphics/Mesh/GLMesh3D.h
	include/graphics/Mesh/GLMeshInterface.h
	include/graphics/Mesh/GLMeshFactory.h
	include/graphics/Mesh/GLMeshData3D.h
	include/graphics/Mesh/GLMeshTriangle3D.h
	include/graphics/Mesh/GLMeshSquare3D.h
	include/graphics/Mesh/GLMeshCube3D.h
	include/graphics/Mesh/GLMeshCircle3D.h
	include/graphics/Mesh/GLMeshSphere3D.h

	# Shader manager
	include/Shaders/ShaderInterface.h
	include/Shaders/ShaderProgram.h
	include/Shaders/ShaderManager.h
	include/Shaders/BasicShader.h

	# # # grid shader  # # #
	include/Shaders/GridShader.h
	# # # light shader # # #
	include/Shaders/LightShader.h

	# Camera stuff
	include/graphics/Camera/Camera.h
	include/graphics/Camera/CameraController.h

	# transformations stuff
	include/graphics/GLTransformations/Transformations.h
	include/graphics/GLTransformations/TransformController.h

	# Grid
	include/graphics/Grid/GridData.h
	include/graphics/Grid/GridRenderer.h
	include/graphics/Grid/GridSystem.h

	# Scene system
	include/Scene/Scene.h
	include/Scene/SceneObject.h
	include/Scene/SceneObjectFactory.h

	# graphic -> render system
	include/graphics/Renderer/RenderData.h
	include/graphics/Renderer/Renderer.h

	# graphic -> Lighting system
	include/graphics/Lighting/Light.h
	include/graphics/Lighting/LightData.h
	include/graphics/Lighting/LightManager.h

	# graphic -> Textures
	include/graphics/Textures/Textures.h
	include/graphics/Textures/stb_image.h

	# graphic -> Material
	include/graphics/Material/MaterialLib.h
)

include_directories(C:/Users/kaay/Desktop/throwEngine/throwEngine/out/build/vs2022-debug/vcpkg_installed/x64-windows/share/stb)

# Copy materials.json to build directory
configure_file(
    "${ASSETS_DIR}/materialJSON/materials.json"
    "${CMAKE_CURRENT_BINARY_DIR}/assets/materialJSON/materials.json"
    COPYONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/include/core/Config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/Config.h
	@ONLY
)

target_include_directories(engine
	PUBLIC
	${SHADERS_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/engine
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(engine
	PUBLIC
	glfw
	glad::glad
	glm::glm
	nlohmann_json::nlohmann_json
)

set_target_properties(engine PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)