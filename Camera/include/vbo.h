#pragma once
#include <glad/glad.h>

class VBO
{
public:
	//Reference ID of the Vertex Buffer Object
	unsigned int ID;

	//Ctor that generates a VBO and links it to vertices
	//NOTE: vertices us being passed by reference, something like vertices[10] however, with explicitly mentioned size within [] is passed by value .i.e a copy of the array is created inside the function
	VBO(float vertices[], int size);

	void Bind();

	void UnBind();

	void Delete(); //As of now, I've never had to use this, maybe I'll need it later?
}
;
