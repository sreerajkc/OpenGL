#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, unsigned int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)// While(GLenum error = glGetError()) is also valid since GL_NO_ERROR is zero
	{
		std::cout << "[OpenGL Error] (" << error << "): "
			<< function << " " << file << ":" << line << std::endl; // prints error in decimal number system
		return false;
	}

	return true;
}

