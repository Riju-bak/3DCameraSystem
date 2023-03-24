#pragma once
#include "glad/glad.h"
#include "vbo.h"

class VAO
{
public:
	//ID reference for the Vertex Array Object
	unsigned int ID;

	VAO();

	void LinkVBO(VBO& vbo, unsigned layout, int stride, int offset = 0, bool is2dTexture = false);


	void Bind();
	void UnBind();
	void Delete();
};

