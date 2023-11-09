#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <windows.h>
#include <algorithm>

#include "Shader.h"


class MyWindow
{
private:
	
	void processInput();

public:
	MyWindow(int height, int width, const char* title, bool resizable);
	MyWindow(const MyWindow&) = delete;
    ~MyWindow();
	MyWindow& operator= (const MyWindow&) = delete;

	void run();
};

