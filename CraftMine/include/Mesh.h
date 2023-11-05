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
    static std::vector<unsigned int> unusedBuffers;
    const glm::vec3 position;
    unsigned int VAO, VBO, texture = 0, elementCount;
    size_t bufferSize;

    void initMesh(std::vector<BufferVertex>& buffer);
    void copyBuffer(std::vector<BufferVertex>& buffer);

public:

    Mesh(std::vector<BufferVertex>& v, size_t b, glm::vec3 p, unsigned int t) : 
        texture{ t }, 
        position{ p }, 
        bufferSize{ b },
        elementCount{ (unsigned int)v.size() }
    { initMesh(v); }

    Mesh(std::vector<BufferVertex>& v, glm::vec3 p, unsigned int t) : 
        texture{ t }, 
        position{ p }, 
        bufferSize{ v.size() * sizeof(BufferVertex) },
        elementCount{ (unsigned int)v.size() }
    { initMesh(v); }
    
    Mesh(const Mesh&) = delete;
    ~Mesh();

    void draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view);
    void updateBuffer(std::vector<BufferVertex>& newBuffer, size_t elementSize);
};
