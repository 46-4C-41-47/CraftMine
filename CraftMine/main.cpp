#include <iostream>
#include "include/Shader.h"

int main() {
	Shader shader("./res/vertexShader.glsl", "./res/fragmentShader.glsl");
	std::cout << "hello world\n";

	return 0;
}