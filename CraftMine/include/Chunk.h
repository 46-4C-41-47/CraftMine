#pragma once

#include <stdlib.h>
#include <time.h>

#include "BlockTypes.h"
#include "Mesh.h"


class Chunk {
private:
	const int MIN_HEIGHT = 128, MAX_HEIGHT = 192;
	Block* chunkData;
	Mesh* mesh;

	void init();
	void addWater();
	void createCaves();
	void generateMesh();

public:
	const int WIDTH = 16, HEIGHT = 256;

	Chunk();
	~Chunk();

	Mesh getMesh();
};