#include "../include/Mesh.h"


Mesh::Mesh(vector<float> v, glm::vec3 p, unsigned int t, Light* l) {
    vertices = v;
    texture = t;
    light = l;
    position = p;

    initMesh();
}


Mesh::Mesh(vector<float> v, Light* l) {
    vertices = v;
    light = l;
    position = l->position;
    isLight = true;

    initMesh();
}


Mesh::~Mesh() {}


void Mesh::initMesh() 
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

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


void Mesh::initLightSource() {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A potentiellement remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}


void Mesh::Draw(Shader& shader, glm::mat4& projection, glm::mat4& view)
{
    shader.use();

    if (isLight) 
    {
        position = light->position;
        shader.sendVec3("color", light->color);
    }
    else 
    {
        shader.sendInt("texture1", 0);
        shader.sendVec3("lightPos", light->position);
        shader.sendVec3("lightColor", light->color);
        shader.sendFloat("ambientStrength", light->ambientStrength);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.sendMat4("model", model);
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    // draw mesh
    glBindVertexArray(VAO);
    
    if (isLight)
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8);
    else 
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glBindVertexArray(0);
}
