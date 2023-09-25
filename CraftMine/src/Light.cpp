#include "../include/Light.h"


Light::Light(glm::vec3 position, glm::vec3 color, float ambient)
{
    this->position = position;
    this->color = color;
    this->ambientStrength = ambient;

    init();
}


Light::~Light() {};


void Light::init() 
{
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


void Light::draw(Shader& shader, glm::mat4& projection, glm::mat4& view)
{
    shader.use();

    position = position;
    shader.sendVec3("color", color);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.sendMat4("model", model);
    shader.sendMat4("view", view);
    shader.sendMat4("projection", projection);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    
    glBindVertexArray(0);
}
