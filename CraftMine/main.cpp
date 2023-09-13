#include <iostream>
#include "include/Shader.h"

int main() {
	std::cout << "hello world" << std::endl;

	Shader shader("./res/vertexShader.glsl", "./res/fragmentShader.glsl");

	return 0;
}