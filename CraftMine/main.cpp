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

    vector<float> verticesVAOcnt = {
      // vertex coordinates   normal vector      texture coordinates
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                                   
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                                                   
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
                                                   
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
                                                   
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
                                                   
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  0.0f,   0.0f, 1.0f
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
        std::cout << "Failed to load texture, path : " << path << std::endl;
    }
    stbi_image_free(data);

    return new Texture { address, width, height };
}


int main(void)
{
    const double delta = 1000.0f / 60;
    const int frameWidth = 800, frameHeight = 500;
    const float aspectRatio = (float)frameWidth / (float)frameHeight;

    double startingTime;

    Shader* shader;
    Mesh* mesh;

    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(frameWidth, frameHeight, "JavaScript = caca", NULL, NULL);

    if (window == NULL)
    {
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

    try
    {
        shader = new Shader("./res/shaders/vertexShader.glsl", "./res/shaders/fragmentShader.glsl");
    }
    catch (std::exception)
    {
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Texture* t = loadTexture("./res/textures/texture.jpg");

    cam = new Camera(vec3(0.0f, 0.0f, 6.0f), vec3(0.0f, 0.0f, 0.0f));
    mesh = new Mesh(cube::verticesVAOcnt, t->id);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput(window, cam);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, -3.0f));

        glm::mat4 view = glm::mat4(1.0f);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 100.0f);

        shader->use();
        shader->sendMat4("model", model);
        shader->sendMat4("view", cam->getViewMatrix());
        shader->sendMat4("projection", projection);

        // draw meshes
        mesh->Draw(*shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(max(delta - ((glfwGetTime() - startingTime) / 1000), 0));
    }

    delete cam, shader;

    glfwTerminate();
    return 0;
}
