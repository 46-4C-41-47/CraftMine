#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <windows.h>
#include <iostream>

#include "include/Shader.h"
#include "include/Camera.h"
#include "include/stb_image.h"


using std::cout;
using std::cerr;
using std::endl;

Camera* cam;


namespace cube 
{
    const float vertices[] = {
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f
    };

    const int indices[] = {
        3, 0, 1, // top
        3, 1, 2,
        4, 6, 5, // bottom
        4, 7, 6,
        4, 0, 3, // front
        4, 3, 7,
        1, 6, 2, // back
        1, 5, 6,
        0, 5, 4, // right
        0, 1, 5,
        2, 7, 3, // left
        2, 6, 7
    };

    float verticesVAO[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    cam->proccessMouse(xpos, ypos);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->moveForward(cam->CAM_SPEED);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->moveSideWays(-cam->CAM_SPEED);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->moveForward(-cam->CAM_SPEED);
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->moveSideWays(cam->CAM_SPEED);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->moveUpward(cam->CAM_SPEED);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->moveUpward(-cam->CAM_SPEED);
}


int main(void)
{
    const double delta = 1000.0f / 60;
    const int frameWidth = 800, frameHeight = 500;
    const float aspectRatio = (float)frameWidth / (float)frameHeight;

    unsigned int VBO, VAO, EBO, texture;

    double startingTime;

    int textureWidth, textureHeight, textureColorChannels;
    unsigned char* textureData;

    Shader* shader;

    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(frameWidth, frameHeight, "JavaScript = caca", NULL, NULL);

    if (window == NULL) {
        cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD" << std::endl;
        return 3;
    }

    glViewport(0, 0, frameWidth, frameHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    textureData = stbi_load(
        "./res/textures/texture.jpg",
        &textureWidth,
        &textureHeight,
        &textureColorChannels,
        0
    );
    glGenTextures(1, &texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if (textureData)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            textureWidth,
            textureHeight,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            textureData
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "unable to load the texture\n";
    }
    stbi_image_free(textureData);

    // VBO
    glGenBuffers(1, &VBO);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube::verticesVAO), cube::verticesVAO, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    try {
        shader = new Shader("./res/shaders/vertexShader.glsl", "./res/shaders/fragmentShader.glsl");
    } catch (std::exception) {
        return 1;
    }
    shader->sendInt("textureData", 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cam = new Camera(vec3(0.0f, 0.0f, 6.0f), vec3(0.0f, 0.0f, 0.0f));

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput(window, cam);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 100.0f);

        shader->use();
        shader->sendMat4("model", model);
        shader->sendMat4("view", cam->getViewMatrix());
        shader->sendMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(max(delta - ((glfwGetTime() - startingTime) / 1000), 0));
    }

    delete cam;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
