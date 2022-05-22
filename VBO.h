#pragma once

#include <glad/glad.h>

class VBO
{
public:
	GLuint ID;

public:
	VBO(GLfloat* vertices, GLsizeiptr size);

	void bind();
	void unbind();
	void destroy();
};
