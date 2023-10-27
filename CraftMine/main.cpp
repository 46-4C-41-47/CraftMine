#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <windows.h>

#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <functional>


#include "include/Mesh.h"
#include "include/Chunk.h"
#include "include/Noise.h"
#include "include/Shader.h"
#include "include/Camera.h"
#include "include/Texture.h"
#include "include/stb_image.h"
#include "include/ThreadPool.h"
#include "include/BufferElement.h"


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


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        lightSource->position = cam->position;
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
    const int frameWidth = 1200, frameHeight = 800;
    const float aspectRatio = (float)frameWidth / (float)frameHeight;

    double startingTime;

    const int tabSize = (params::chunk::RADIUS * 2 + 1) * (params::chunk::RADIUS * 2 + 1);

    Chunk* visibleChunks[tabSize] = { nullptr };
    Shader* objectShader, *lightShader;

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 200.0f);

    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(frameWidth, frameHeight, "CraftMine", NULL, NULL);
    glfwSetWindowPos(window, 1920, 200);

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
    Light* light = new Light(vec3(0.0f, 180.0f, -5.0f), vec3(0.99f, 0.99f, 0.99f), 0.6f);
    
    cam = new Camera(vec3(15.0f, 150.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f));
    glm::vec2 previousPos;

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput(window, cam, light);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        previousPos = Chunk::updateChunks(visibleChunks, light, previousPos, cam->position, t->id);
//      ^~~~~~~~~~~ TODO trouver une alternative pour ce systeme chelou

        glm::mat4 view = cam->getViewMatrix();

        light->draw(*lightShader, projection, view);

        for (int i = 0; i < tabSize; i++)
        {
            if (visibleChunks[i] != nullptr)
                visibleChunks[i]->draw(*objectShader, projection, view);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(max(delta - ((glfwGetTime() - startingTime) * 1000), 0));
    }

    delete cam, objectShader, lightShader, visibleChunks;

    glfwTerminate();
    return 0;
}


/*int main()
{
    const double delta = 1000.0f / 60;
    const int frameWidth = 1200, frameHeight = 800;
    const float aspectRatio = (float)frameWidth / (float)frameHeight;

    double startingTime;

    Shader* objectShader, * lightShader;

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 200.0f);

    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(frameWidth, frameHeight, "CraftMine", NULL, NULL);
    glfwSetWindowPos(window, 1920, 200);

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
    Light* light = new Light(vec3(0.0f, 180.0f, -5.0f), vec3(0.99f, 0.99f, 0.99f), 0.6f);

    cam = new Camera(vec3(15.0f, 150.0f, 15.0f), vec3(0.0f, 0.0f, 0.0f));
    glm::vec2 previousPos;

    Chunk* c1 = new Chunk(0, 0, light, t->id);
    Chunk* c2 = new Chunk(1, 0, light, t->id);

    for (int i = 0; i < params::chunk::WIDTH; i++)
    {
        for (int j = 0; j < params::chunk::HEIGHT; j++)
        {
            if (!c1->isThereABlock(params::chunk::WIDTH - 1, j, i) && c2->isThereABlock(0, j, i))
            {
                c2->buffer->insertFace(0, j, i, 2);
                std::cout << "insert z : " << i << ", y : " << j << "\n";
            }
        }
    }
    double start = glfwGetTime();
    c2->updateMesh();
    double end = (glfwGetTime() - start) * 1000;
    std::cout << "update time : " << end << " ms\n";

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput(window, cam, light);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam->getViewMatrix();

        light->draw(*lightShader, projection, view);

        c1->draw(*objectShader, projection, view);
        c2->draw(*objectShader, projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(max(delta - ((glfwGetTime() - startingTime) * 1000), 0));
    }

    delete cam, objectShader, lightShader;

    glfwTerminate();
    return 0;
}*/


/*int main()
{
    glfwInit();

    double start, avg, sum = 0, d = 1000000;

    for (int i = 0; i < d; i++)
    {
        start = glfwGetTime();


        sum += (glfwGetTime() - start) * 1000;
    }

    avg = sum / d;

    cout << "average elapsed time : " << avg << "\n";

    return 0;
}*/

/*
TODO :
classe Joueur
ajouter un readme sur git
faire une doc si j'ai pas la flemme
rendre asynchrone la generation des chunks
soft reload des chunks
finir la classe camera
prise en charge de textures différentes pour les blocs (finir la classe Block)
ajouter des grottes
rendre le décor destructible
ajouter des nuages
ajouter un cycle jour nuit
*/
