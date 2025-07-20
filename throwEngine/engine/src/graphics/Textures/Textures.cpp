#include "graphics/Textures/Textures.h"

#include "core/Logger.h"

namespace TEXTURE
{
	Textures::Textures()
		: m_width(2), m_height(2), m_nrChannels(0)
	{
	}
	
	void Textures::createTexture()
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// add texture into the our texture unit vector!
		addTextureIntoIDs(texture);
	}

	bool Textures::loadImage(const std::string& path)
	{
		glBindTexture(GL_TEXTURE_2D, textureIDs.back());

		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			GLint internalFormat = GL_RGB8;

			if (m_nrChannels == 1) {
				format = GL_RED;
				internalFormat = GL_R8;
			}
			else if (m_nrChannels == 3) {
				format = GL_RGB;
				internalFormat = GL_RGB8;
			}
			else if (m_nrChannels == 4) {
				format = GL_RGBA;
				internalFormat = GL_RGBA8;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
	
			stbi_image_free(data);
			return true;
		}
		else {
			std::cout << "Failed to load texture!" << "\n";
			stbi_image_free(data);
			return false;
		}
	}

	uint32_t Textures::generateTexture(const std::string& filePath)
	{
		createTexture();
		if (!loadImage(filePath)) {
			Logger::warn("[Textures::generateTexture] loadImage failed!");
			return 0;
		}
		return textureIDs.back();
	}

	void Textures::bind(uint32_t diffuseTexID, uint32_t specularTexID)
	{
		if (diffuseTexID != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseTexID);
		}

		if (specularTexID != 0) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularTexID);
		}
	}

	uint32_t Textures::getTexID(int index) const
	{
		return textureIDs[index];
	}

	void Textures::addTextureIntoIDs(uint32_t texID)
	{
		textureIDs.push_back(texID);
	}
}