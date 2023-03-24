#pragma once
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

class Texture2D
{
public:
	//ID reference for the texture
	unsigned int ID;

	Texture2D(const char* textureImgPath);

	//Bind the texture
	void Bind(unsigned int slot /* =0 */);

	//select active textureUnit(to make the texture available to fragment shader as a uniform variable)
	void MakeActive(unsigned int textureUnit);

	std::string GetImageExtension(const char* textureImgPathc);

	int GetPixelDataFormat(const char* textureImgPath);

};
