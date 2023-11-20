#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <system_error>


class Shader
{
private: 
    unsigned int finalShader;

    unsigned int initShaderStep(const std::string& codePath, GLenum shaderType);
    unsigned int compileShaderStep(const std::string& shaderCode, GLenum shaderType);
    void assertLinking();
    std::string getShaderCode(const std::string& shaderPath);

public:
    unsigned int ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
    
    Shader(const Shader&) = delete;

    Shader& operator = (const Shader&) = delete;

    inline unsigned int getId() { return finalShader; }


    void use();
    void sendVec2(const std::string& name, const glm::vec2& value);
    void sendVec3(const std::string& name, const glm::vec3& value);
    void sendMat4(const std::string& name, const glm::mat4& value);
    void sendFloat(const std::string& name, float value);
    void sendInt(const std::string& name, int value);
};
