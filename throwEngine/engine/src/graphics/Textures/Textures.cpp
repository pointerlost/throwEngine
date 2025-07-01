#include "graphics/Textures/Textures.h"

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// add texture into the our texture unit vector!
		addTextureIntoIDs(texture);
	}

	void Textures::loadImage(const std::string& path)
	{
		glBindTexture(GL_TEXTURE_2D, textureIDs.back());

		unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			if (m_nrChannels == 1)
				format = GL_RED;
			else if (m_nrChannels == 3)
				format = GL_RGB;
			else if (m_nrChannels == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
	
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture!" << "\n";
			stbi_image_free(data);
		}
	}

	uint32_t Textures::generateTexture(const std::string& filePath)
	{
		int textureUnit = textureIDs.size();
		createTexture();
		loadImage(filePath);
		return textureUnit;
	}

	void Textures::bind()
	{
		for (int i = 0; i < textureIDs.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
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