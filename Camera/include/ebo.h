#pragma once
#include <glad/glad.h>

class EBO
{
public:
	unsigned int ID;

	EBO(unsigned int indices[], int size);

	void Bind();

	void UnBind();

	void Delete();
};

