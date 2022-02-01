#include <glad/glad.h>

#include "inc\ErrorHandler.h"

void GLClearError()
{
	while (glGetError() != 0);
}

bool GLCheckError(const char* code, const char* file, int line)
{
	bool noErrorFlag = true;
	unsigned int errorCode;

	while ((errorCode = glGetError()) != 0) {
		std::cout << "OpenGL Error Occured (ID: " << errorCode << ")" << std::endl;
		std::cout << "File \"" << file << "\", line " << line << std::endl;
		
		std::string errorMsg;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:					errorMsg = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:					errorMsg = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:				errorMsg = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:					errorMsg = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:				errorMsg = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:					errorMsg = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	errorMsg = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		std::cout << "\033[1;31m" << errorMsg << "\033[0m: in \"" << code << "\"" << std::endl;
		noErrorFlag = false;
	}

	return noErrorFlag;
}