#pragma once
#include <GL/glew.h>


#define ASSERT(x) if(!(x)) __debugbreak(); // Compiler intrinsic assertion
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, unsigned int line);
