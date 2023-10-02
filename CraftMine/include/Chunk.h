#pragma once

#include <cstdlib>
#include <stdlib.h>
#include <time.h>

#include <glm/glm.hpp>

#include <iostream>
#include <algorithm>
#include <vector>

#include "Block.h"
#include "Mesh.h"
#include "Shader.h"


class Chunk {
private:
	const int MIN_HEIGHT = 128, MAX_HEIGHT = 192, HEIGHT_RANGE = 8;
    unsigned int texture, chunkDataSize;
    bool needToUpdate = true;
	std::vector<Block::ChunkBlock>* blocks;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    Mesh* mesh;
	Light* light;
	Block::Type* chunkData;

	void init();
	void addWater();
	void createCaves();
	double* getHeightMap();
	void generateMesh();
	Block::Type* getBlock(int x, int y, int z);
	inline int getIndex(int x, int y, int z) { return x + (y * WIDTH) + (z * WIDTH * HEIGHT); }

public:
	const int WIDTH = 64, HEIGHT = 32;

	Chunk(Light* l, unsigned int t);
	~Chunk();

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);
};