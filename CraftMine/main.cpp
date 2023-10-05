#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "include/Shader.h"
#include "include/Camera.h"
#include "include/stb_image.h"
#include "include/Mesh.h"
#include "include/Texture.h"
#include "include/Chunk.h"
#include "include/Noise.h"


using std::cout;
using std::cerr;
using std::endl;
using std::unique_ptr;

Camera* cam;


void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    cam->proccessMouse(xpos, ypos);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Camera* cam, Light* lightSource)
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


    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightSource->position.x += 0.02f;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightSource->position.x -= 0.02f;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightSource->position.z += 0.02f;

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightSource->position.z -= 0.02f;

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        lightSource->position.y += 0.02f;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        lightSource->position.y -= 0.02f;
}


Texture* loadTexture(std::string path) {
    unsigned int address;
    int width, height, nrChannels;
    unsigned char* data;

    glGenTextures(1, &address);
    glBindTexture(GL_TEXTURE_2D, address);
    
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load image, create texture and generate mipmaps
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cerr << "Failed to load texture, path : " << path << "\n";
    }
    stbi_image_free(data);

    return new Texture { address, width, height };
}


int main()
{
    const double delta = 1000.0f / 60;
    const int frameWidth = 800, frameHeight = 500;
    const float aspectRatio = (float)frameWidth / (float)frameHeight;

    double startingTime;

    const int tabSize = (Chunk::RADIUS * 2 + 1) * (Chunk::RADIUS * 2 + 1);
    Chunk* visibleChunks[tabSize] = { nullptr };

    Shader* objectShader, *lightShader;

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 100.0f);

    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(frameWidth, frameHeight, "CraftMine", NULL, NULL);

    if (window == NULL)
    {
        cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD\n";
        return 3;
    }

    glViewport(0, 0, frameWidth, frameHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    try
    {
        objectShader = new Shader(
            "./res/shaders/vertexShader.glsl", 
            "./res/shaders/fragmentShader.glsl"
        );
        lightShader = new Shader(
            "./res/shaders/light/lightVertexShader.glsl", 
            "./res/shaders/light/lightFragmentShader.glsl"
        );
    }
    catch (std::exception)
    {
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Texture* t = loadTexture("./res/textures/texture.jpg");

    cam = new Camera(vec3(15.0f, 150.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f));

    Light* light = new Light(vec3(0.0f, 180.0f, -5.0f), vec3(0.99f, 0.99f, 0.99f), 0.2f);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput(window, cam, light);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Chunk::updateChunks(visibleChunks, cam->position, light, t->id);
        
        glm::mat4 view = cam->getViewMatrix();

        // draw meshes
        light->draw(*lightShader, projection, view);

        for (int i = 0; i < tabSize; i++)
            visibleChunks[i]->draw(*objectShader, projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(max(delta - ((glfwGetTime() - startingTime) / 1000), 0));
    }

    delete cam, objectShader, lightShader, visibleChunks;

    glfwTerminate();
    return 0;
}
