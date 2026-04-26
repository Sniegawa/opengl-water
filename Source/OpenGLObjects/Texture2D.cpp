#include "Texture2D.h"

#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D()
{
	glGenTextures(1, &m_ID);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}

Texture2D::Texture2D(const std::string& path)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum internalFormat;
		GLenum format;

		switch (nrChannels)
		{
		case 1:
			internalFormat = GL_RED;
			break;
		case 2:
			internalFormat = GL_RG;
			break;
		case 3:
			internalFormat = GL_RGB;
			break;
		case 4:
			internalFormat = GL_RGBA;
			break;
		default:
			internalFormat = GL_RGBA;
			break;
		}

		format = internalFormat; // TODO : change


		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}