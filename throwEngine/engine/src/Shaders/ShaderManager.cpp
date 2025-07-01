#include "Shaders/ShaderManager.h"

#include "core/File.h"

#include "Shaders/ShaderProgram.h"
#include "Shaders/GridShader.h"
#include "Shaders/LightShader.h"
#include "Shaders/BasicShader.h"

#include "config.h"
#include <core/Logger.h>
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = core::File::get();

namespace SHADER
{

    bool ShaderManager::loadAllShaders() {
        const std::string configPath = SHADERS_CONFIG_PATH;
        Logger::info("Loading shaders from config: " + configPath);

        try {
            std::string jsonContent = file.readFromFile(configPath);
            auto config = nlohmann::json::parse(jsonContent);

            if (!config.contains("shaders")) {
                throw std::runtime_error("Missing 'shaders' array in config");
            }

            for (auto& shaderConfig : config["shaders"]) {

                std::string name     = shaderConfig.value("name", "unnamed");
                std::string vertFile = shaderConfig["vertex"].get<std::string>();
                std::string fragFile = shaderConfig["fragment"].get<std::string>();
                bool isHelper        = shaderConfig.value("helper", false);

                if (!loadShader(name, vertFile, fragFile, isHelper, getType(shaderConfig))) {
                    Logger::error("Failed to load shader: " + name);
                }
            }
            return true;
        }
        catch (...) {
            Logger::error("Failed to load shader config, using fallback");
            return createFallbackShader();
        }
    }

    bool ShaderManager::loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath, bool isHelper, ShaderType type)
    {
        try
        {
            auto fullVertPath = std::string(SHADERS_DIR) + "/GLSL/" + vertPath;
            auto fullFragPath = std::string(SHADERS_DIR) + "/GLSL/" + fragPath;

            auto vertSource = file.readFromFile(fullVertPath);
            auto fragSource = file.readFromFile(fullFragPath);

            auto shaderProg = std::make_shared<GLShaderProgram>(vertSource, fragSource);
            DEBUG_PTR(shaderProg);

            if (isHelper) {
                setWrapperGLShader(shaderProg);
                Logger::info("[ShaderManager] Set helper shader: " + name);
            }

            std::shared_ptr<IShader> shaderInterface;
            switch (type)
            {
            case ShaderType::LIGHT:
                shaderInterface = std::make_shared<LightShader>(shaderProg);
                break;
            case ShaderType::GRID:
                shaderInterface = std::make_shared<GridShader>(shaderProg);
                break;
            default:
                shaderInterface = std::make_shared<BasicShader>(shaderProg);
            }

            addShaderInterface(name, shaderInterface);
            return true;
        }
        catch (const std::exception& e) {
            Logger::error("Failed to load shader " + name + ": " + e.what());
            return false;
        }
    }

    bool ShaderManager::createFallbackShader() {
        const std::string vertPath = std::string(CMAKE_CURRENT_BINARY_DIR) + "/GLSL/basic.vert";
        const std::string fragPath = std::string(CMAKE_CURRENT_BINARY_DIR) + "/GLSL/basic.frag";

        Logger::info("Attempting fallback shader at:\n  " + vertPath + "\n  " + fragPath);

        try {
            auto vertSource = file.readFromFile(vertPath);
            auto fragSource = file.readFromFile(fragPath);

            auto shaderProg = std::make_shared<GLShaderProgram>(vertSource, fragSource);
            auto fallback = std::make_shared<BasicShader>(shaderProg);

            addShaderInterface("fallback", fallback);
            setWrapperGLShader(shaderProg);
            return true;
        }
        catch (const std::exception& e) {
            Logger::error("CRITICAL: Failed to create fallback shader: " +
                std::string(e.what()));
            return false;
        }
    }

    ShaderType ShaderManager::getType(const nlohmann::json& shaderConfig)
    {
        std::string typeStr = shaderConfig.value("type", "basic");

        if (typeStr == "light") return ShaderType::LIGHT;
        if (typeStr == "grid")  return ShaderType::GRID;
        return ShaderType::BASIC;
    }

	void ShaderManager::addShaderInterface(const std::string& name, const std::shared_ptr<IShader>& shader)
	{
		auto it = shaderInterfaces_.find(name);

		if (it != shaderInterfaces_.end()) {
			Logger::warn("Shader interface '" + name + "' already exists. Skipping addition.");
		}
		else {
			shaderInterfaces_[name] = shader;
			Logger::info("Shader interface '" + name + "' added successfully.");
		}
	}

	std::shared_ptr<IShader> ShaderManager::getShaderInterface(const std::string& name)
	{
        if (shaderInterfaces_.empty()) {
            Logger::warn("shaderInterfaces_ is empty!");
        }
		auto it = shaderInterfaces_.find(name);

		if (it != shaderInterfaces_.end()) {
            return it->second;
        }

		Logger::error("Shader interface '" + name + "' not found!");

		static auto fallbackShader = [this](){
            const std::string vertSource = std::string(SHADERS_DIR) + "/GLSL/basic.vert";
			const std::string fragSource = std::string(SHADERS_DIR) + "/GLSL/basic.frag";
			auto program = std::make_shared<GLShaderProgram>(vertSource, fragSource);
            setWrapperGLShader(program);
			return std::make_shared<BasicShader>(program);
		}();

		return fallbackShader;
	}

	std::vector<std::shared_ptr<IShader>> ShaderManager::getAllShaderInterfaces()
	{
		std::vector<std::shared_ptr<IShader>> shaderVector;

		for (const auto& it : shaderInterfaces_) {
			shaderVector.push_back(it.second);
		}

		if (shaderVector.empty())
			Logger::warn("No shader interfaces loaded!");

		return shaderVector;
	}

}