#include "../include/Mesh.h"


Mesh::Mesh(vector<BufferVertex>& v, glm::vec3 p, unsigned int t)
{
    texture = t;
    position = p;

    bufferSize = v.size();

    initMesh(v);
}


Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void Mesh::initMesh(vector<BufferVertex>& buffer)
{

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(BufferVertex), &buffer[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_INT, GL_FALSE, sizeof(BufferVertex), (void*)0);

    // start at 1 to skip the id which is an int not a float
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
    shader.sendVec3("lightPos", light.position);
    shader.sendVec3("lightColor", light.color);
    shader.sendFloat("ambientStrength", light.ambientStrength);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.sendMat4("model", model);
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, bufferSize);

    glBindVertexArray(0);
}


void Mesh::setBuffer(vector<BufferVertex>& newBuffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, newBuffer.size() * sizeof(float), &newBuffer[0], GL_DYNAMIC_DRAW);
}
