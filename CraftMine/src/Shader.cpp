#include "../include/Shader.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    finalShader = glCreateProgram();

    unsigned int vertexShader = initShaderStep(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = initShaderStep(fragmentPath, GL_FRAGMENT_SHADER);

    glLinkProgram(finalShader);

    assertLinking();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


Shader::Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath) 
{
    finalShader = glCreateProgram();

    unsigned int vertexShader = initShaderStep(vertexPath, GL_VERTEX_SHADER);
    unsigned int geometryShader = initShaderStep(geometryPath, GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = initShaderStep(fragmentPath, GL_FRAGMENT_SHADER);

    glLinkProgram(finalShader);

    assertLinking();

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}


unsigned int Shader::initShaderStep(const std::string& codePath, GLenum shaderType)
{
    std::string code;

    try 
    {
        code = getShaderCode(codePath);
    }
    catch (std::exception)
    {
        throw std::exception("unable to create the shader object");
    }

    unsigned int id = compileShaderStep(code, shaderType);

    glAttachShader(finalShader, id);

}


unsigned int Shader::compileShaderStep(const std::string& shaderCode, GLenum shaderType)
{
    int  success;
    char infoLog[512];
    unsigned int shaderId;
    const char* sc = shaderCode.c_str();

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &(sc), NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
        exit(1);
    }

    return shaderId;
}


void Shader::assertLinking()
{
    int  success;
    char infoLog[512];

    glGetProgramiv(finalShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(finalShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << "\n";
        exit(1);
    }
}


std::string Shader::getShaderCode(const std::string& shaderPath)
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(shaderPath);
        std::stringstream vShaderStream;

        vShaderStream << shaderFile.rdbuf();

        shaderFile.close();

        shaderCode = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FAIL_TO_READ_FILE\n";
        exit(1);
    }

    return shaderCode;
}


void Shader::sendVec2(const std::string& name, const glm::vec2& value)
{
    glUniform3fv(glGetUniformLocation(finalShader, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::sendVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(finalShader, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::sendMat4(const std::string& name, const glm::mat4& value) 
{
    glUniformMatrix4fv(glGetUniformLocation(finalShader, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::sendFloat(const std::string& name, float value) 
{
    glUniform1f(glGetUniformLocation(finalShader, name.c_str()), value);
}


void Shader::sendInt(const std::string& name, int value) 
{
    glUniform1i(glGetUniformLocation(finalShader, name.c_str()), value);
}


void Shader::use() { glUseProgram(finalShader); }
