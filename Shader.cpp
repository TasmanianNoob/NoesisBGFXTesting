//
// Created by Charlie on 5/12/2022.
//

#include <regex>
#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"

void checkCompileErrors(GLuint shader, const char* path, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << path << '\n';
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << path << '\n';
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		std::stringstream vShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
	}
	catch(std::ifstream::failure& e)
	{
		std::cout << "VERTEX PATH: " << vertexPath << '\n';
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: VERTEX" << std::endl;
	}

	vertexCode = std::regex_replace(vertexCode, std::regex("(?:#define CHUNK_SIZE )[0-9]+"), "#define CHUNK_SIZE " + std::to_string(32));

	try
	{
		// open files
		fShaderFile.open(fragmentPath);
		std::stringstream fShaderStream;
		// read file’s buffer contents into streams
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		fShaderFile.close();
		// convert stream into string
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure& e)
	{
		std::cout << "FRAGMENT PATH: " << fragmentPath << '\n';
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: FRAGMENT" << std::endl;
	}

	this->vertexShader = vertexCode;
	this->fragmentShader = fragmentCode;
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	/**
	 * VERTEX SHADER
	 */

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, vertexPath, "VERTEX");

	/**
	 * FRAGMENT SHADER
	 */

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, fragmentPath, "FRAGMENT");

	/**
	 * SHADER LINKING
	 */

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "", "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

std::string Shader::GetVertexShader() const
{
	return vertexShader;
}

std::string Shader::GetFragmentShader() const
{
	return fragmentShader;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetBool(std::string&& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, std::move(name).c_str()), value);
}

void Shader::SetInt(std::string&& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, std::move(name).c_str()), value);
}

void Shader::SetFloat(std::string&& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, std::move(name).c_str()), value);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::SetVec3(const std::string &name, glm::vec3 &&vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3 {x, y, z}));
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat4(const std::string &name, glm::mat4 &&mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::AttachGeometryShader(const char* geometryPath) const
{
	std::string vertexCode;
	std::ifstream vShaderFile;

	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(geometryPath);
		std::stringstream vShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();

	/**
	 * VERTEX SHADER
	 */

	uint32_t vertexShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, geometryPath, "GEOMETRY");

	/**
	 * SHADER LINKING
	 */

	glAttachShader(ID, vertexShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "", "PROGRAM");

	glDeleteShader(vertexShader);
}

void Shader::SetVec2(const std::string &name, glm::vec2 &&vec) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &vec) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::SetVec2(const std::string &name, float x, float y) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec2 {x, y}));
}

int Shader::CompileShader(const char *path, GLenum type)
{
	std::string code;
	std::ifstream shaderFile;

	shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(path);
		std::stringstream vShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		code = vShaderStream.str();
	}
	catch(std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: VERTEX" << std::endl;
	}

	const char* shaderCode = code.c_str();

	/**
	 * SHADER
	 */

	uint32_t shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, path, "VERTEX");

	/**
	 * SHADER LINKING
	 */

	ID = glCreateProgram();

	glAttachShader(ID, shader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "", "PROGRAM");

	return shader;
}
