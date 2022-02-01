#pragma once

#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCALL(x) \
	/*GLClearError();*/\
	x;\
	ASSERT(GLCheckError(#x, __FILE__, __LINE__));


void GLClearError();
bool GLCheckError(const char* code, const char *file, int line);