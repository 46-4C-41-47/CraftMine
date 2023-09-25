#include "../include/Mesh.h"


Mesh::Mesh(vector<float>& v, glm::vec3 p, unsigned int t) 
{
    texture = t;
    position = p;

    bufferSize = v.size();

    initMesh(v);
}


Mesh::~Mesh() {}


void Mesh::initMesh(vector<float>& buffer) 
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

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
    glDrawArrays(GL_TRIANGLES, 0, bufferSize / 8);

    glBindVertexArray(0);
}


void Mesh::setBuffer(vector<float>& newBuffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, newBuffer.size() * sizeof(float), &newBuffer[0], GL_STATIC_DRAW);
}
