#include "inc\Shader.h"
#include "inc\ErrorHandler.h"

unsigned int Shader::GenerateShader(const unsigned int shaderType, const std::string& source)
{
	GLCALL(unsigned int shaderID = glCreateShader(shaderType));

	const char* cvtSource = source.c_str();
	GLCALL(glShaderSource(shaderID, 1, &cvtSource, NULL));
	GLCALL(glCompileShader(shaderID));

	int success;
	GLCALL(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));

	// if compilation failed
	if (success == GL_FALSE) {
		//get length
		int length;
		GLCALL(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

		char* errMsg = (char*)alloca(length * sizeof(char));
		GLCALL(glGetShaderInfoLog(shaderID, 512, NULL, errMsg));
		std::cout << "[ID: " << shaderID << "] ERROR:"
			<< (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " Shader did not compile.\n" << errMsg << std::endl;
	}
	else {
		std::cout << "[ID: " << shaderID << "] "
			<< (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " Shader compilation successful." << std::endl;
	}

	return shaderID;
}

unsigned int Shader::GenerateProgram(const unsigned int vertexShader, const unsigned int fragmentShader)
{
	// Shader program
	GLCALL(unsigned int shaderProgram = glCreateProgram());

	// attach the shader objects to the program
	GLCALL(glAttachShader(shaderProgram, vertexShader));
	GLCALL(glAttachShader(shaderProgram, fragmentShader));
	GLCALL(glLinkProgram(shaderProgram));

	int success;
	GLCALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));

	// if compilation failed
	if (success == GL_FALSE) {
		//get length
		int length;
		GLCALL(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length));

		char* errMsg = (char*)alloca(length * sizeof(char));
		GLCALL(glGetProgramInfoLog(shaderProgram, 512, NULL, errMsg));
		std::cout << "[ID: " << shaderProgram << "]"
			<< " ERROR: Program did not link.\n" << errMsg << std::endl;
	}

	else {
		std::cout << "[ID: " << shaderProgram << "]"
			<< " Program linked successful." << std::endl;
	}

	return shaderProgram;
}

std::string Shader::ReadShaderFile(const std::string &filePath)
{
	std::stringstream source;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(filePath);
		
		source << file.rdbuf();

		file.close();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR: FILE \"" << filePath << "\" DID NOT SUCCESFULLY READ." << std::endl;
		std::cout << e.what() << std::endl;
	}

	return source.str();
}

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	std::string vertexShaderSource, fragmentShaderSource;

	vertexShaderSource = ReadShaderFile(vertexFilePath);
	fragmentShaderSource = ReadShaderFile(fragmentFilePath);

	// Create Shader
	unsigned int vertexShader = GenerateShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = GenerateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// Create program and use the program
	this->m_ID = GenerateProgram(vertexShader, fragmentShader);

	//Delete the created shader objects
	GLCALL(glDeleteShader(vertexShader));
	GLCALL(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(this->m_ID));
;}

void Shader::use()
{

	GLCALL(glUseProgram(this->m_ID));
}

void Shader::SetUniformInt(const std::string& name, int i0, int i1, int i2, int i3)
{
	GLCALL(glUniform4i(glGetUniformLocation(this->m_ID, name.c_str()), i0, i1, i2, i3));
}

void Shader::SetUniformFloat(const std::string& name, float f0, float f1, float f2, float f3)
{
	GLCALL(glUniform4f(glGetUniformLocation(this->m_ID, name.c_str()), f0, f1, f2, f3));
}

void Shader::SetUniformInt(const std::string& name, int i0, int i1, int i2)
{
	GLCALL(glUniform3i(glGetUniformLocation(this->m_ID, name.c_str()), i0, i1, i2));
}

void Shader::SetUniformFloat(const std::string& name, float f0, float f1, float f2)
{
	GLCALL(glUniform3f(glGetUniformLocation(this->m_ID, name.c_str()), f0, f1, f2));
}

void Shader::SetUniformInt(const std::string& name, int i0)
{
	GLCALL(glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), i0));
}

void Shader::SetUniformFloat(const std::string& name, float f0)
{
	GLCALL(glUniform1f(glGetUniformLocation(this->m_ID, name.c_str()), f0));
}

void Shader::SetUniformMat4(const std::string& name, const float* matrix)
{
	GLCALL(glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, matrix));
}

