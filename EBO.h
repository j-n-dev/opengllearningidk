#pragma once

#include <glad/glad.h>

class EBO
{
public:
	GLuint ID;

public:
	EBO(GLuint* indices, GLsizeiptr size);

	void bind();
	void unbind();
	void destroy();
};
