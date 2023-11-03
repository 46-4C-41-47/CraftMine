#include "../include/Mesh.h"


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void Mesh::allocateMemory(std::vector<BufferVertex>& buffer)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ Potentiellement a remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    memcpy(ptr, &buffer[0], sizeof(BufferVertex) * buffer.size());
    glUnmapBuffer(GL_ARRAY_BUFFER);
}


void Mesh::initMesh(std::vector<BufferVertex>& buffer)
{
    bool reuseBuffer = true;

    if (unusedBuffers.size() == 0)
    {
        glGenBuffers(1, &VBO);
    }
    else
    {
        VBO = unusedBuffers[unusedBuffers.size() - 1];
        unusedBuffers.pop_back();
    }

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (!reuseBuffer)
        allocateMemory(buffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_INT, GL_FALSE, sizeof(BufferVertex), (void*)0);

    // commence a 1 pour passer l'id
    int byte_offset = 1;
    for (int i = 1; i < BV_DISPOSITION_SIZE; i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            BV_DISPOSITION[i],
            GL_FLOAT,
            GL_FALSE,
            sizeof(BufferVertex),
            (void*)(byte_offset * sizeof(float))
        );

        byte_offset = byte_offset + BV_DISPOSITION[i];
    }
    
    glBindVertexArray(0);
}


void Mesh::draw(Shader& shader, Light& light, glm::mat4& projection, glm::mat4& view)
{
    shader.use();

    shader.sendInt("texture1", 0);
    shader.sendVec3("fogColor", params::graphical::FOG_COLOR);
    shader.sendVec3("lightPos", light.position);
    shader.sendVec3("lightColor", light.color);
    shader.sendFloat("ambientStrength", light.ambientStrength);
    shader.sendFloat("nearPlane", params::graphical::NEAR_PLANE);
    shader.sendFloat("farPlane", params::graphical::FAR_PLANE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.sendMat4("model", model);
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, elementCount);

    glBindVertexArray(0);
}


void Mesh::updateBuffer(std::vector<BufferVertex>& newBuffer, size_t elementSize)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newBuffer.size() * elementSize, &newBuffer[0]);
}
