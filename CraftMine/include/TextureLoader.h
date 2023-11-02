#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

#include "Texture.h"
#include "stb_image.h"


class TextureLoader
{
private:
	static TextureLoader* instance;

	static const std::vector<std::string> paths;
	std::vector<Texture*> textures;

	Texture* loadTexture(std::string path);

	TextureLoader();

public:
	static TextureLoader* getInstance();

	TextureLoader(const TextureLoader&) = delete;
	~TextureLoader();

	unsigned int getTexture(int index) { return textures[index]->id; }
	static int getTextureX(int textureIndex);
	static int getTextureY(int textureIndex);
};
