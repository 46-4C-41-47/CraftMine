#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "stb_image.h"



struct Texture {
    unsigned int id;
    int width;
    int height;
};
