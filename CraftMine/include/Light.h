#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Shader.h"


class Light
{
private:
    std::vector<float> vertices = {
          -0.5f, -0.5f, -0.5f, // front
           0.5f, -0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f, -0.5f,
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f, -0.5f,  0.5f, // back
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,

          -0.5f,  0.5f,  0.5f, // left
          -0.5f,  0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f, -0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,

           0.5f,  0.5f,  0.5f, // right
           0.5f,  0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,

          -0.5f, -0.5f, -0.5f, // bottom
           0.5f, -0.5f, -0.5f,
           0.5f, -0.5f,  0.5f,
           0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f,  0.5f,
          -0.5f, -0.5f, -0.5f,

          -0.5f,  0.5f, -0.5f, // top
           0.5f,  0.5f, -0.5f,
           0.5f,  0.5f,  0.5f,
           0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f,  0.5f,
          -0.5f,  0.5f, -0.5f
    };
    unsigned int VAO, VBO;

    void init();

public:
	glm::vec3 position;
	glm::vec3 color;
	float ambientStrength;

	Light(glm::vec3 position, glm::vec3 color, float ambient);
	~Light();

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);
};
