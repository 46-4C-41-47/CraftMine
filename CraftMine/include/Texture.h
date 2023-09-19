#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "stb_image.h"



class Texture {
private:
	unsigned int texture;

public:
	Texture(std::string path);

	inline unsigned int getAddress() { return texture; }
	inline void free() { glDeleteTextures(1, &texture); }
};
