#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

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
#include "include/TextureHandler.h"

#define max(a, b) ((a > b) ? a : b)

using params::graphical::SKY_COLOR;


Player p1(glm::vec3(15.0f, 150.0f, 15.0f));


void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    p1.getCam().proccessMouse(xpos, ypos);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Camera& cam, Light& lightSource, double delta)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double camSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camSpeed = params::controls::CAM_SPEED * 2.0 * (delta / 1000000);
    else
        camSpeed = params::controls::CAM_SPEED * (delta / 1000000);
    

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.moveForward(camSpeed);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.moveSideWays(-camSpeed);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.moveForward(-camSpeed);
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.moveSideWays(camSpeed);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.moveUpward(camSpeed);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.moveUpward(-camSpeed);


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        lightSource.position = cam.position;
}


int main()
{
    //double startingTime, sleepDuration;
    std::chrono::high_resolution_clock::time_point start, end;
    double delta = 0;

    const int tabSize = (params::chunk::RADIUS * 2 + 1) * (params::chunk::RADIUS * 2 + 1);

    Chunk* visibleChunks[tabSize] = { nullptr };
    Shader* objectShader, * lightShader;

    glm::mat4 view;
    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f), 
        params::graphical::ASPECT_RATIO, 
        params::graphical::NEAR_PLANE, 
        params::graphical::FAR_PLANE
    );

    if (!glfwInit())
    {
        std::cerr << "Initialization of GLFW failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(params::graphical::FRAME_WIDTH, params::graphical::FRAME_HEIGHT, "CraftMine", NULL, NULL);
    glfwSetWindowPos(window, 2625, 200);

    if (window == NULL)
    {
        std::cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return 3;
    }

    glViewport(0, 0, params::graphical::FRAME_WIDTH, params::graphical::FRAME_HEIGHT);

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

    TextureHandler* th = TextureHandler::getInstance();
    Light light(vec3(0.0f, 180.0f, -5.0f), vec3(0.99f, 0.9f, 0.9f), 0.6f);

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        start = std::chrono::high_resolution_clock::now();

        processInput(window, p1.getCam(), light, delta);

        glClearColor(SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Chunk::updateChunks(visibleChunks, light, p1, th->getTexture());

        view = p1.getCam().getViewMatrix();

        light.draw(*lightShader, projection, view);

        for (int i = 0; i < tabSize; i++)
        {
            if (visibleChunks[i] != nullptr)
                visibleChunks[i]->draw(*objectShader, projection, view);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        end = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        // std::this_thread::sleep_for(std::chrono::microseconds((int)max(params::graphical::FRAME_INTERVAL - delta, 0))); // limite de framerate
        // comment c'est possible de faire un langage aussi illisible j'ai les yeux qui brûlent
    }

    delete th, objectShader, lightShader, visibleChunks, window;

    glfwTerminate();
    return 0;
}


/*int main()
{
    glfwInit();

    double start, avg, sum = 0, d = 10000;

    for (int i = 0; i < d; i++)
    {
        start = glfwGetTime();

        // stuff that needs to be measured

        sum += (glfwGetTime() - start) * 1000;
    }

    avg = sum / d;

    cout << "average elapsed time : " << avg << " ms\n";
    cout << d << " iterations\n";
    char a;
    std::cin >> a;

    glfwTerminate();
    return 0;
}*/


/*
À FAIRE  :
trouver la fuite de mémoire (classe Mesh)
ajouter un readme sur git
faire une doc si j'ai pas la flemme
rendre asynchrone la generation des chunks
finir la classe camera
prise en charge de textures différentes pour les blocs (finir la classe Block)
ajouter des grottes
rendre le décor destructible
ajouter des nuages
ajouter un cycle jour nuit
*/
