#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <windows.h>
#include <algorithm>

#include "Shader.h"


class MyWindow
{
private:
	const double delta = 1000.0f / 60;
	float aspectRatio;
    unsigned int compiledShader;
	const char* vertexShaderPath = "./vertexShader.glsl";
	const char* fragmentShaderPath = "./fragmentShader.glsl";
	GLFWwindow* window;
    Shader* shader;

	void processInput();

public:
	MyWindow(int height, int width, const char* title, bool resizable);
	MyWindow(const MyWindow&) = delete;
    ~MyWindow();
	MyWindow& operator= (const MyWindow&) = delete;

	void run();
};

