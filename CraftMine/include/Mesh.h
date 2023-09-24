#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "Shader.h"
#include "Light.h"


using std::vector;


class Mesh {
private:
    bool isLight = false;
    unsigned int VAO, VBO, texture = 0;
    Light* light;
    vector<float> vertices;

    void initMesh();
    void initLightSource();

public:
    glm::vec3 position;

    Mesh(vector<float> v, glm::vec3 p, unsigned int t, Light* l);
    Mesh(vector<float> v, Light* l);
    ~Mesh();

    void Draw(Shader& shader, glm::mat4& projection, glm::mat4& view);
};
