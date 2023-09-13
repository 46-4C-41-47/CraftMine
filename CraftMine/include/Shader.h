#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
private: 
    unsigned int finalShader;

    unsigned int initShader(const char* shaderCode, GLenum shaderType);
    std::string getShaderCode(const char* shaderPath);

public:
    unsigned int ID;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    Shader(const Shader&) = delete;
    Shader& operator = (const Shader&) = delete;

    inline unsigned int getId() { return finalShader; }

    void use();
    void sendMat4(const std::string& name, const glm::mat4& value);
    void sendInt(const std::string& name, int value);
};
