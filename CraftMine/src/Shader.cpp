#include "../include/Shader.h"


using std::cerr;
using std::endl;
using std::string;


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) 
{
    int  success;
    char infoLog[512];
    string vertexCode, fragmentCode;

    try {
        vertexCode = getShaderCode(vertexShaderPath);
        fragmentCode = getShaderCode(fragmentShaderPath);
    } catch (std::exception) {
        throw std::exception("unable to create the shader object");
    }

    unsigned int vertexShader = initShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = initShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    finalShader = glCreateProgram();

    glAttachShader(finalShader, vertexShader);
    glAttachShader(finalShader, fragmentShader);
    glLinkProgram(finalShader);

    glGetProgramiv(finalShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(finalShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


string Shader::getShaderCode(const char* shaderPath) 
{
    string shaderCode;
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
        cerr << "ERROR::SHADER::FAIL_TO_READ_FILE" << endl;
        throw std::exception("unable to load the shader file");
    }

    return shaderCode;
}


unsigned int Shader::initShader(const char* shaderCode, GLenum shaderType) 
{
    int  success;
    char infoLog[512];
    unsigned int shaderId;

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
        return NULL;
    }

    return shaderId;
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
