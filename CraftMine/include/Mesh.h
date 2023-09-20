#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO, EBO;

    vector<Vertex>* vertices;
    //vector<unsigned int> indices;
    vector<unsigned int>* textures;

    void initMesh();

public:
    Mesh(float* rawData, vector<unsigned int>* textures);
    Mesh(vector<Vertex>* vertices, vector<unsigned int>* textures);
    void Draw(Shader& shader);
};
