#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Light.h"
#include "Shader.h"
#include "BufferElement.h"
#include "ChunkMeshBuffer.h"


using std::vector;


class Mesh {
private:
    unsigned int VAO, VBO, bufferSize, texture = 0;

    void initMesh(vector<BufferVertex>& buffer);

public:
    glm::vec3 position;

    Mesh(vector<BufferVertex>& v, glm::vec3 p, unsigned int t);
    ~Mesh();

    void draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view);
    void setBuffer(vector<BufferVertex>& newBuffer);
};
