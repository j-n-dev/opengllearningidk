#include <iostream>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Shader.h"
#include "VAO.h"
#include "VAO.h"
#include "EBO.h"

#define  WIDTH 800
#define HEIGHT 800

//void GLAPIENTRY
//OH(GLenum source,
//	GLenum type,
//	GLuint id,
//	GLenum severity,
//	GLsizei length,
//	const GLchar* message,
//	const void* userParam)
//{
//	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
//		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
//		type, severity, message);
//}

int main()
{
	// initialize GLFW
	glfwInit();

	// tell him what version of OpenGL we are using (in this case 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // tell him we are using the CORE profile
	                                                               // that means we only have the modern functions

	// create a GLFWwindow of 640 by 480 pixels
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "L", NULL, NULL);
	if (window == NULL) // error checking magic
	{
		std::cout << "uh oh a stinky" << std::endl;
		glfwTerminate();
		return 1;
	}
	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	gladLoadGL(); // load in GLAD and configure OpenGL

	// set up debug message callback
	/*glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OH, NULL);*/

	glViewport(0, 0, WIDTH, HEIGHT); // specify the OpenGL viewport in the window

	// the vertices for the triangle
	GLfloat vertices[] =
	{//      VERTICES        |       COLORS      | TEXTURE COORDS //
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // (0)
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // (1)
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // (2)
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,    1.0f, 0.0f   // (3)
	};

	GLuint indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	Shader shaderProgram("default.vert", "default.frag");

	// set up the VAO, VBO and EBO
	VAO vao1;
	vao1.bind();

	VBO vbo1(vertices, sizeof(vertices));
	EBO ebo1(indices, sizeof(indices));

	vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.linkAttrib(vbo1, 1, 3, GL_FLOAT, 8 *sizeof(float), (void*)(3 * sizeof(float)));
	vao1.linkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	// unbind 'em all like pokemon cards
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	// gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// texture
	int imgWidth, imgHeight, colChanNum;
	unsigned char* bytes = stbi_load("him.png", &imgWidth, &imgHeight, &colChanNum, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.activate();
	glUniform1i(tex0Uni, 0);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // specify the color of the background
		glClear(GL_COLOR_BUFFER_BIT); // set the color of the background
		shaderProgram.activate(); // activate shader program
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		vao1.bind(); // bind the VAO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // clean the back buffer

		glfwPollEvents(); // take care of events
	}

	vao1.destroy();
	vbo1.destroy();
	ebo1.destroy();
	shaderProgram.destroy();
	glDeleteTextures(1, &texture);

	// delete window
	glfwDestroyWindow(window);
	glfwTerminate(); // terminate GLFW
	return 0;
}
