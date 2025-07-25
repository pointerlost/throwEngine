#include "core/File.h"

#include "core/Logger.h"

namespace core
{
    File& File::get()
    {
        static File instance;
        return instance;
    }

    std::string File::readFromFile(const std::string& name) {

        std::ifstream file(name, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            Logger::error("File not found: " + name);
            return "";
        }

        // Dosya boyutunu al
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string content(size, '\0');
        if (!file.read(&content[0], size)) {
            Logger::error("Error reading file: " + name);
            return "";
        }

        return content;
    }


	bool File::exists(const std::string& path) const noexcept
	{
		std::ifstream f(path);
		return f.good();
	}
}