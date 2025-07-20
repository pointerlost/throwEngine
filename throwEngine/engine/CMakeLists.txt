cmake_minimum_required(VERSION 3.21)
project(engine)

# compiler standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set paths for shaders and assets early so they can be used in includes and configure_file
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shaders")
set(SHADER_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Shaders")
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(SHADERS_CONFIG_PATH "${SHADERS_DIR}/configs/shaders.json")

# define static engine library
add_library(engine STATIC)

# define header and source paths
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
        ${SHADERS_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/engine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${VCPKG_ROOT}/installed/x64-windows/include
        
)

target_compile_features(engine PUBLIC cxx_std_23)

# Core
target_sources(engine PRIVATE
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/File.cpp

    include/core/Engine.h
    include/core/Window.h
    include/core/File.h
    include/core/Logger.h
)

# Input
target_sources(engine PRIVATE
    src/Input/Input.cpp
    src/Input/InputContext.cpp
    src/Input/InputComponent.cpp
    src/Input/InputComponentFactory.cpp

    include/Input/Input.h
    include/Input/InputContext.h
    include/Input/InputComponent.h
    include/Input/InputComponentFactory.h
)

# Callback
target_sources(engine PRIVATE
    src/CallBack/CallBack.cpp
    include/CallBack/CallBack.h
)

# Meshes
target_sources(engine PRIVATE
    src/graphics/Mesh/GLMesh3D.cpp
    src/graphics/Mesh/GLMeshFactory.cpp
    src/graphics/Mesh/GLMeshData3D.cpp
    src/graphics/Mesh/GLMeshTriangle3D.cpp
    src/graphics/Mesh/GLMeshSquare3D.cpp
    src/graphics/Mesh/GLMeshCube3D.cpp
    src/graphics/Mesh/GLMeshCircle3D.cpp
    src/graphics/Mesh/GLMeshSphere3D.cpp

    include/graphics/Mesh/GLMesh3D.h
    include/graphics/Mesh/GLMeshInterface.h
    include/graphics/Mesh/GLMeshFactory.h
    include/graphics/Mesh/GLMeshData3D.h
    include/graphics/Mesh/GLMeshTriangle3D.h
    include/graphics/Mesh/GLMeshSquare3D.h
    include/graphics/Mesh/GLMeshCube3D.h
    include/graphics/Mesh/GLMeshCircle3D.h
    include/graphics/Mesh/GLMeshSphere3D.h
)

# Shader System
target_sources(engine PRIVATE
    src/Shaders/ShaderProgram.cpp
    src/Shaders/ShaderManager.cpp
    src/Shaders/BasicShader.cpp
    src/Shaders/GridShader.cpp
    src/Shaders/LightShader.cpp

    include/Shaders/ShaderInterface.h
    include/Shaders/ShaderProgram.h
    include/Shaders/ShaderManager.h
    include/Shaders/BasicShader.h
    include/Shaders/GridShader.h
    include/Shaders/LightShader.h
)

# Copy shader files to output directory
file (GLOB GLSL_FILES   "${SHADERS_DIR}/GLSL/*")
file (GLOB CONFIG_FILES "${SHADERS_DIR}/configs/*")

file(COPY ${GLSL_FILES} DESTINATION ${SHADER_OUTPUT_DIR}/GLSL)
file(COPY ${CONFIG_FILES} DESTINATION ${SHADER_OUTPUT_DIR}/configs)

# Camera
target_sources(engine PRIVATE
    src/graphics/Camera/Camera.cpp
    src/graphics/Camera/CameraController.cpp

    include/graphics/Camera/Camera.h
    include/graphics/Camera/CameraController.h
)

# Grid
target_sources(engine PRIVATE
    src/graphics/Grid/GridData.cpp
    src/graphics/Grid/GridRenderer.cpp
    src/graphics/Grid/GridSystem.cpp

    include/graphics/Grid/GridData.h
    include/graphics/Grid/GridRenderer.h
    include/graphics/Grid/GridSystem.h
)

# Renderer
target_sources(engine PRIVATE
    src/graphics/Renderer/RenderData.cpp
    src/graphics/Renderer/Renderer.cpp

    include/graphics/Renderer/RenderData.h
    include/graphics/Renderer/Renderer.h
)

# Lighting
target_sources(engine PRIVATE
    src/graphics/Lighting/Light.cpp
    src/graphics/Lighting/LightData.cpp
    src/graphics/Lighting/LightManager.cpp

    include/graphics/Lighting/Light.h
    include/graphics/Lighting/LightData.h
    include/graphics/Lighting/LightManager.h
)

# Scene
target_sources(engine PRIVATE
    src/Scene/Scene.cpp
    src/Scene/SceneObject.cpp
    src/Scene/SceneObjectFactory.cpp

    include/Scene/Scene.h
    include/Scene/SceneObject.h
    include/Scene/SceneObjectFactory.h
)

# Material & Textures
target_sources(engine PRIVATE
    src/graphics/Material/MaterialLib.cpp
    include/graphics/Material/MaterialLib.h

    src/graphics/Textures/Textures.cpp
    src/graphics/Textures/stb_image.cpp
    include/graphics/Textures/Textures.h
    include/graphics/Textures/stb_image.h
)

# Material source files & assets stuff
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets")

# Copy textures directory into build/assets/
file(COPY "${ASSETS_DIR}/textures" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")

# Check materials.json exists
if(NOT EXISTS "${ASSETS_DIR}/materialJSON/materials.json")
    message(FATAL_ERROR "Materials JSON file not found at: ${ASSETS_DIR}/materialJSON/materials.json")
endif()

# Copy materials.json to build directory
configure_file(
    "${ASSETS_DIR}/materialJSON/materials.json"
    "${CMAKE_CURRENT_BINARY_DIR}/assets/materialJSON/materials.json"
    COPYONLY
)

# ImGui Layer
target_sources(engine PRIVATE
    src/ImGui/ImGuiLayer.cpp
    src/ImGui/ImGuiScoped.cpp
    src/ImGui/ImGuiObjectState.cpp

    include/ImGui/ImGuiLayer.h
    include/ImGui/ImGuiScoped.h
    include/ImGui/ImGuiObjectState.h
)

# Transformations
target_sources(engine PRIVATE
    src/graphics/GLTransformations/Transformations.cpp
    src/graphics/GLTransformations/TransformController.cpp

    include/graphics/GLTransformations/Transformations.h
    include/graphics/GLTransformations/TransformController.h
)

# Configure Config.h from Config.h.in
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/Config.h
    @ONLY
)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Link libraries
target_link_libraries(engine
    PUBLIC
    glfw
    glad::glad
    glm::glm
    nlohmann_json::nlohmann_json
    imgui::imgui
)

# Set debugger working directory for Visual Studio
set_target_properties(engine PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/engine"
)
