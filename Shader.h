#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

/// <summary>Gets the contents of a specific file.</summary>/// <param name="filename">The path and name of the file</param>/// <returns>The contents of the file</returns>
std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;

public:
	Shader(const char* vertex, const char* fragment);

	void activate();
	void destroy();

private:
	void compileErrors(unsigned int shader, const char* type);
};