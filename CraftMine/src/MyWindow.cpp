#include "../include/MyWindow.h"


using std::cerr;
using std::endl;

/*void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}*/


MyWindow::MyWindow(int width, int height, const char* title, bool resizable) {
    if (!glfwInit())
    {
        cerr << "Initialization of GLFW failed\n";
        return;
    }

    if (!resizable) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL) {
        cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return;
    }

    glViewport(0, 0, width, height);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    aspectRatio = (float)width / (float)height;
    shader = new Shader(vertexShaderPath, fragmentShaderPath);

    glEnable(GL_DEPTH_TEST);
}


MyWindow::~MyWindow() {
    delete window;
    delete shader;
    glfwTerminate();
}


void MyWindow::run() {
    double startingTime;

    while (!glfwWindowShouldClose(window))
    {
        startingTime = glfwGetTime();

        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * 0.7913f, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 100.0f);

        shader->use();
        shader->sendMat4("model", model);
        shader->sendMat4("view", view);
        shader->sendMat4("projection", projection);

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, sizeof(cube::indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        Sleep(std::max(delta - ((glfwGetTime() - startingTime) / 1000), 0.0));*/
    }

    glfwTerminate();
}


void MyWindow::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
