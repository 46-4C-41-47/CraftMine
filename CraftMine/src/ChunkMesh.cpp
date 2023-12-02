#include "../include/ChunkMesh.h"


ChunkMesh::~ChunkMesh() {
    glDeleteVertexArrays(1, &VAO);
}


void ChunkMesh::initMesh(FaceBuffer& faceBuffer)
{
    size_t vertexSize = 8 * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &faceDataVBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(float) * data::cube_faces[faceBuffer.faceType].size(),
        &data::cube_faces[faceBuffer.faceType][0],
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, faceDataVBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(face) * faceBuffer.getData().size(), 
        faceBuffer.getData().data(),
        GL_STATIC_DRAW
    );

    // données invariables
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(6 * sizeof(float)));

    // données variables
    glBindBuffer(GL_ARRAY_BUFFER, faceDataVBO);
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
    glVertexAttribDivisor(3, 1);
    
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_INT, GL_FALSE, sizeof(int), (void*)(3 * sizeof(int)));
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void ChunkMesh::draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view)
{
    shader.use();
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elementCount);

    glBindVertexArray(0);
}
