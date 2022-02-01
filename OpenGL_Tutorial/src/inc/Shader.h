#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>		
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{
private:
	unsigned int m_ID;

	unsigned int GenerateShader(const unsigned int shaderType, const std::string& source);
	unsigned int GenerateProgram(const unsigned int vertexShader, const unsigned int fragmentShader);

	std::string ReadShaderFile(const std::string& filePath);

public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	// use/activate shader
	void use();

	// utility uniform functions
	void SetUniformInt	(const std::string &name, int i0, int i1, int i2, int i3);
	void SetUniformFloat(const std::string &name, float f0, float f1, float f2, float f3);

	void SetUniformInt	(const std::string& name, int i0, int i1, int i2);
	void SetUniformFloat(const std::string& name, float f0, float f1, float f2);

	void SetUniformInt(const std::string& name, int i0);
	void SetUniformFloat(const std::string& name, float f0);

	// utility uniform matrix function
	void SetUniformMat4(const std::string& name, const float* matrix);


};