#include "shader.h"

std::string GetFileContents(const char* filePath)
{
	std::ifstream file;
	file.open(filePath, std::ios::in);	 //ios::in implies Open file for reading only
	if (!file)
	{
		std::cerr << "Unable to open: " << filePath << std::endl;
		exit(2);
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


int GenVertexShader(const char* vertexFile)
{
	std::string shaderString = GetFileContents(vertexFile);
	int shaderStringLen = shaderString.size();

	const char* vertexShaderSource = shaderString.c_str();

	// Next we attach the shader source code to the shader object and compile the shader :
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, &shaderStringLen);
	glCompileShader(vertexShader);

	//Check if compilation was successful
	// int success;
	// glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// if (!success)
	// {
	// 	char infoLog[512];
	// 	glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
	// 	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	// }
	return vertexShader;
}


int GenFragmentShader(const char* fragmentFile)
{
	const std::string shaderString = GetFileContents(fragmentFile);
	const int shaderStringLen = shaderString.size();
	const char* fragmentShaderSource = shaderString.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, &shaderStringLen);
	glCompileShader(fragmentShader);

	//Check if compilation is successful
	// int success;
	// glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// if (!success)
	// {
	// 	char infoLog[512];
	// 	glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
	// 	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	// }
	return fragmentShader;
}


Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	int vertexShader = GenVertexShader(vertexFile);

	int fragmentShader = GenFragmentShader(fragmentFile);


	//A shader program object is the final linked version of multiple shaders combined
	// To use the recently
	// 	compiled shaders we have to link them to a shader program object and then activate this shader
	// 	program when rendering objects
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);

	int success;
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(this->ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}


	// Oh yeah, and don’t forget to delete the shader objects once we’ve linked them into the program object;
	// we no longer need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::SetUniformi(const char* name, int value)
{
	int location = glGetUniformLocation(this->ID, name);
	glUniform1i(location, value);
}


void Shader::SetUniformf(const char* name, float value)
{
	int location = glGetUniformLocation(this->ID, name);
	glUniform1f(location, value);
}

void Shader::SetUniformMatrix4fv(const char* name, glm::mat4& transform)
{
	int location = glGetUniformLocation(this->ID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::Activate()
{
	glUseProgram(this->ID);
}

void Shader::Delete()
{
	glDeleteProgram(this->ID);
}
