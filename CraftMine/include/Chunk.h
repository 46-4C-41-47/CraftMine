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
#include "Player.h"
#include "Shader.h"
#include "Parameters.h"
#include "BufferElement.h"
#include "TextureHandler.h"
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

using constants::BlockType;

using constants::texture::CHUNK_WIDTH;

using params::chunk::WIDTH;
using params::chunk::HEIGHT;
using params::chunk::SPREAD;
using params::chunk::RADIUS;
using params::chunk::HEIGHT_RANGE;
using params::chunk::MAX_MEM_SPACE;

using func::getChunkIndex;
using func::getBufferId;
using func::getUIndex;
using func::getVIndex;


class Chunk 
{
private:
	static std::vector<Chunk*>* visibleChunks;

	const TextureHandler* th = TextureHandler::getInstance();
	    
	bool initStatus = false, bordersFullyUpdated = false;
	unsigned int texture, chunkDataSize;
	
	Light& light;
    Mesh* mesh;
	Chunk* neighbors[4]; /* north south east west */
	BlockType* chunkData;
	ChunkMeshBuffer* buffer;

	std::future<ChunkMeshBuffer*> asyncBuffer;
	std::mutex meshMutex, bufferMutex;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	void init();
	void addWater();
	void createCaves();
	void updateBorders();
	BlockType getBlock(int x, int y, int z);
	ChunkMeshBuffer* generateMesh();

public:
	const int x, y;

	Chunk(int x, int y, Light& l, unsigned int t);
	Chunk(const Chunk&) = delete;
	~Chunk();

	bool isThereABlock(int x, int y, int z);
	bool getInitStatus() { return initStatus; }

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);
	void setNeighbor(Chunk** value);
	void updateMesh();

	const std::vector<Chunk*>& getVisibleChunks() { return visibleChunks; }

	static void updateChunks(Chunk** visibleChunks, Light& l, Player& p, unsigned int t);
};
