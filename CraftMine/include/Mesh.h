#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO;
    size_t sizeOfVertices;

    vector<float> vertices;
    vector<unsigned int> textures;

    void initMesh();

public:
    Mesh(vector<float> v);
    ~Mesh();

    void Draw(Shader& shader);
};
