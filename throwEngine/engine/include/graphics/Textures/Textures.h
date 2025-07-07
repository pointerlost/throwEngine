#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Textures/stb_image.h"

namespace TEXTURE
{
	class Textures
	{
	public:
		Textures();

		void createTexture();
		bool loadImage(const std::string& path);
		uint32_t generateTexture(const std::string& filePath);
		
		void bind(uint32_t diffuseTexID, uint32_t specularTexID);

		uint32_t getTexID(int index) const;

		const std::vector<uint32_t> getTextureVector() const { return textureIDs; };

		void addTextureIntoIDs(uint32_t texID);

	private:

		std::vector<uint32_t> textureIDs;

		int m_width;
		int m_height;
		int m_nrChannels;
	};
}