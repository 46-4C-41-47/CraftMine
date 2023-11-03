#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

#include "Texture.h"
#include "stb_image.h"
#include "Parameters.h"


class TextureHandler final
{
private:
	static TextureHandler* instance;

	static const std::string path;
	Texture* texture;

	Texture* loadTexture(std::string path);

	TextureHandler() { texture = loadTexture(path); }

public:
	static TextureHandler* getInstance();

	TextureHandler(const TextureHandler&) = delete;
	~TextureHandler() { delete texture; }

	unsigned int getTexture() { return texture->id; }

	inline float getVIndex(int position, float offset)
	{
		return (((int)(position / constants::texture::CHUNK_COUNT))
			/
			(float)constants::texture::CHUNK_COUNT)
			+ offset * constants::texture::CHUNK_WIDTH;
	}

	inline float getUIndex(int position, float offset)
	{
		return ((position % constants::texture::CHUNK_COUNT)
			/
			(float)constants::texture::CHUNK_COUNT)
			+ offset * constants::texture::CHUNK_WIDTH;
	}
};
