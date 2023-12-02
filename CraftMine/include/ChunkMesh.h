#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Light.h"
#include "Shader.h"
#include "FaceBuffer.h"
#include "Parameters.h"
#include "BufferElement.h"
#include "ChunkMeshBuffer.h"


class ChunkMesh {
private:
    unsigned int VAO, faceDataVBO, VBO, texture = 0, elementCount;

    void initMesh(FaceBuffer& faceBuffer);

public:
    ChunkMesh(FaceBuffer& fb, unsigned int t) : texture{ t }, elementCount{ (unsigned int)fb.getData().size()}
    { initMesh(fb); }
    
    ChunkMesh(const ChunkMesh&) = delete;
    ~ChunkMesh();

    void draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view);
};
