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
#include "Parameters.h"
#include "ThreadPool.h"
#include "BufferElement.h"
#include "ChunkMeshBuffer.h"

/*
				NORTH
		---------------------
	  y	|					|
		|					|
		|					|
   WEST	|		CHUNK		| EAST
		|					|
		|					|
		|					|
		--------------------- x
				SOUTH
*/

using func::getChunkIndex;
using func::getBufferId;

using params::chunk::WIDTH;
using params::chunk::HEIGHT;
using params::chunk::SPREAD;
using params::chunk::RADIUS;
using params::chunk::HEIGHT_RANGE;


class Chunk {
private:
	static ThreadPool* threadPool;
	    
	unsigned int texture, chunkDataSize;
    bool updatingMesh = false;
	
	Chunk* neighbors[4]; /* north south east west */
    Mesh* mesh;
	Light* light;
	Block::Type* chunkData;

	std::mutex meshMutex;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	void init();
	void addWater();
	void updateBorders(int borderIndex);
	void createCaves();
	void generateMesh();
	Block::Type getBlock(int x, int y, int z);

public:
	const int x, y;
	ChunkMeshBuffer* buffer;

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
