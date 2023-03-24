#include "vao.h"

VAO::VAO()
{
	glGenVertexArrays(1, &this->ID);
}

void VAO::LinkVBO(VBO& vbo, unsigned int layout, int stride = 3 * sizeof(float), int offset, bool is2dTexture)
{
	vbo.Bind();
	int nPoints = 3;
	if (is2dTexture)
		nPoints = 2;
	glVertexAttribPointer(layout, nPoints, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(layout);
	vbo.UnBind();
}

void VAO::Bind()
{
	glBindVertexArray(this->ID);
}

void VAO::UnBind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &this->ID);
}
