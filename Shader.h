//
// Created by Charlie on 5/12/2022.
//

#ifndef OPENGLBOOKVS_SHADER_H
#define OPENGLBOOKVS_SHADER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "glad.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Shader
{
public:
	// the program ID
	uint32_t ID;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void Use() const;
    int CompileShader(const char* path, GLenum type);
    void AttachGeometryShader(const char* geometryPath) const;
    
    std::string GetVertexShader() const;
    std::string GetFragmentShader() const;
	// utility uniform functions
    void SetVec2(const std::string &name, const glm::vec2 &vec) const;
    void SetVec2(const std::string &name, glm::vec2 &&vec) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec3(const std::string &name, const glm::vec3 &vec) const;
    void SetVec3(const std::string &name, glm::vec3 &&vec) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;
    void SetMat4(const std::string &name, glm::mat4 &&mat) const;
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetBool(std::string &&name, bool value) const;
	void SetInt(std::string &&name, int value) const;
	void SetFloat(std::string &&name, float value) const;
private:
    std::string vertexShader;
    std::string fragmentShader;
};

#endif //OPENGLBOOKVS_SHADER_H
