#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO, texture;
    size_t sizeOfVertices;

    vector<float> vertices;

    void initMesh();

public:
    Mesh(vector<float> v, unsigned int t);
    ~Mesh();

    void Draw(Shader& shader);
};
