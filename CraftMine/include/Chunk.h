#pragma once

#include <stdlib.h>
#include <time.h>

#include <vector>

#include "Block.h"
#include "Mesh.h"


class Chunk {
private:
	const int MIN_HEIGHT = 128, MAX_HEIGHT = 192;
	Block::Type* chunkData;
	vector<Mesh>* mesh;

	void init();
	void addWater();
	void createCaves();
	void generateMesh();

public:
	const int WIDTH = 16, HEIGHT = 256;

	Chunk();
	~Chunk();

	vector<Mesh> getMesh();
};