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

using constants::NORTH;
using constants::SOUTH;
using constants::EAST;
using constants::WEST;

using constants::FRONT;
using constants::BACK;
using constants::LEFT;
using constants::RIGHT;
using constants::BOTTOM;
using constants::TOP;

using params::chunk::WIDTH;
using params::chunk::HEIGHT;
using params::chunk::SPREAD;
using params::chunk::RADIUS;
using params::chunk::HEIGHT_RANGE;

using func::getChunkIndex;
using func::getBufferId;


class Chunk {
private:
	static ThreadPool* threadPool;
	    
	unsigned int texture, chunkDataSize;
	
	Light& light;
    Mesh* mesh;
	Chunk* neighbors[4]; /* north south east west */
	Block::Type* chunkData;
	ChunkMeshBuffer* buffer;

	std::future<ChunkMeshBuffer*> asyncBuffer;
	std::mutex meshMutex, bufferMutex;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	void init();
	void addWater();
	void createCaves();
	void updateBorders();
	Block::Type getBlock(int x, int y, int z);
	ChunkMeshBuffer* generateMesh();

public:
	const int x, y;

	Chunk(int x, int y, Light& l, unsigned int t);
	Chunk(const Chunk&) = delete;
	~Chunk();

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);

	static glm::vec2 updateChunks(
		Chunk** visibleChunks, 
		Light& l, 
		const glm::vec2& previousPos, 
		const glm::vec3& pos, 
		unsigned int t
	);
	bool isThereABlock(int x, int y, int z);
	void setNeighbor(Chunk** value);
	void updateMesh();
};
