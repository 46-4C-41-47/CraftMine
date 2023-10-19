#pragma once

#include <time.h>
#include <cstdlib>
#include <stdlib.h>

#include <glm/glm.hpp>

#include <mutex>
#include <vector>
#include <future>
#include <iostream>
#include <algorithm>

#include "Mesh.h"
#include "Noise.h"
#include "Block.h"
#include "Shader.h"
#include "ThreadPool.h"
#include "BufferElement.h"
#include "ChunkMeshBuffer.h"


class Chunk {
private:
	static const int cubeFaceSize = 48;
	static const std::vector<float> cubeVerticesVAOcnt;
	static ThreadPool* threadPool;
	
	const int MIN_HEIGHT = 128, MAX_HEIGHT = 192, HEIGHT_RANGE = 64;
	const double noiseFrequency = 0.15413, noiseSeed = 684.6565;
    
	unsigned int texture, chunkDataSize;
    bool updatingMesh = false;
	
	ChunkMeshBuffer* buffer;
	Chunk* neighbors[4]; /* north south east west */
    Mesh* mesh;
	Light* light;
	Block::Type* chunkData;

	std::mutex meshMutex;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	void init();
	void addWater();
	void updateMesh();
	void createCaves();
	void generateMesh();
	Block::Type getBlock(int x, int y, int z);
	inline int getIndex(int x, int y, int z) { return x + (y * WIDTH) + (z * WIDTH * HEIGHT); }

public:
	static const int WIDTH = 16, HEIGHT = 256, RADIUS = 8, SPREAD = 4;
	const int x, y;

	Chunk(int x, int y, Light* l, unsigned int t);
	~Chunk();

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);

	static glm::vec2 updateChunks(
		Chunk** visibleChunks, 
		Light* l, 
		const glm::vec2& previousPos, 
		const glm::vec3& pos, 
		unsigned int t
	);
	bool isThereABlock(int x, int y, int z);

	void setNeighbor(Chunk** value);
};
