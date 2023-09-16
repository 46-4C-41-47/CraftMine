#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Vertex.h"
#include "Texture.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO, EBO;

    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    void setupMesh();

public:
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    //void Draw(Shader& shader);
};
