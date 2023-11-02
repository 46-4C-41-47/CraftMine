#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Light.h"
#include "Shader.h"
#include "Parameters.h"
#include "BufferElement.h"
#include "ChunkMeshBuffer.h"


class Mesh {
private:
    unsigned int VAO, VBO, bufferSize, texture = 0;

    void initMesh(std::vector<BufferVertex>& buffer);

public:
    glm::vec3 position;

    Mesh(std::vector<BufferVertex>& v, glm::vec3 p, unsigned int t) : texture{ t }, position{ p }, bufferSize{ (unsigned int)v.size() }
    { initMesh(v); }
    Mesh(const Mesh&) = delete;
    ~Mesh();

    void draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view);
    void updateBuffer(std::vector<BufferVertex>& newBuffer, size_t elementSize);
};
