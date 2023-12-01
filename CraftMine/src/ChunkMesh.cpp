#include "../include/ChunkMesh.h"

namespace cube_faces {
    const std::vector<float> front = {
        //    vertex coordinates  normal vector         texture coordinates
        0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // front
        1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    };
    const std::vector<float> back = {
        0.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // back
        1.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
    };
    const std::vector<float> top = {
        0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // top
        1.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };
    const std::vector<float> bottom = {
        0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // bottom
        1.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
    };
    const std::vector<float> left = {
        0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // left
        0.0f, 1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    };
    const std::vector<float> right = {
        1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // right
        1.0f, 1.0f, 0.0f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    };
}

ChunkMesh::~ChunkMesh() {
    unusedBuffers.push_back(VBO);
    glDeleteVertexArrays(1, &VAO);
}


void ChunkMesh::initMesh(std::vector<face>& faceBuffer)
{
    unsigned int faceVBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &faceVBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, faceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(face) * faceBuffer.size(), &faceBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, dataVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
}


void ChunkMesh::draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view)
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.sendMat4("model", model);
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    shader.sendVec3("fogColor", params::graphical::FOG_COLOR);
    shader.sendVec3("lightPos", light.position);
    shader.sendVec3("lightColor", light.color);
    
    shader.sendFloat("fogStrength", params::scene::fogStrength);
    shader.sendFloat("windowWidth", params::graphical::FRAME_WIDTH);
    shader.sendFloat("ambientStrength", light.ambientStrength);
    shader.sendFloat("nearPlane", params::graphical::NEAR_PLANE);
    shader.sendFloat("farPlane", params::graphical::FAR_PLANE);
    
    shader.sendInt("texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, elementCount);

    glBindVertexArray(0);
}


void ChunkMesh::updateBuffer(std::vector<BufferVertex>& newBuffer, size_t elementSize)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newBuffer.size() * elementSize, &newBuffer[0]);
}
