#include "Texture2D.h"

Texture2D::Texture2D(const char* textureImgPath)
{
	//This is needed because for an image, origin is at top-left, whereas for OpenGL origin is at bottom left
	stbi_set_flip_vertically_on_load(1);

	// Load image file into memory
	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureImgPath, &width, &height, &nrChannels, 0);

	//Generate and bind texture object
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	//Set texture parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //in context of textures (s,t,r) => (x,y,z) axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture data
	if (data != nullptr)
	{
		int pixelDataFormat = GetPixelDataFormat(textureImgPath);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelDataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << std::endl;
	}

	//Free image data as it's no longer required
	stbi_image_free(data);
}

void Texture2D::Bind(unsigned int slot /* =0 */)
{
	//Example
	// glActiveTexture(GL_TEXTURE0);  //make the texture-unit/slot GL_TEXTURE0 active
	// glBindTexture(GL_TEXTURE_2D, texture);	//bind texture to target GL_TEXTURE_2D

	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture2D::MakeActive(unsigned int textureUnit)
{
	//Example
	// glActiveTexture(GL_TEXTURE0);  //make the texture-unit/slot GL_TEXTURE0 active
	//NOTE: texture-unit and slot refer to the same thing
	glActiveTexture(textureUnit);
}

std::string Texture2D::GetImageExtension(const char* textureImgPathc)
{
	std::string textureImgPath = std::string(textureImgPathc);
	return textureImgPath.substr(textureImgPath.size() - 3);
}

int Texture2D::GetPixelDataFormat(const char* textureImgPath)
{
	int pixelDataFormat = GL_RGB;
	std::string imgExtension = GetImageExtension(textureImgPath);
	if (imgExtension == "png")
	{
		pixelDataFormat = GL_RGBA;
	}
	return pixelDataFormat;
}
