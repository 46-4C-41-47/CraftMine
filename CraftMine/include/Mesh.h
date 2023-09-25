#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Shader.h"
#include "Light.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO, bufferSize, texture = 0;

    void initMesh(vector<float>& buffer);

public:
    glm::vec3 position;

    Mesh(vector<float>& v, glm::vec3 p, unsigned int t);
    ~Mesh();

    void draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view);
    void setBuffer(vector<float>& newBuffer);
};
