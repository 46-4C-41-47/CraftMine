#include "../include/Chunk.h"


Chunk::Chunk() {
	chunkData = new Block[WIDTH * WIDTH * HEIGHT];
}


Chunk::~Chunk() {
	delete[] chunkData;
}


void Chunk::init() {
	srand(time(NULL));

	for (int z = 0; z < HEIGHT; z++) {
		for (int y = 0; y < WIDTH; y++) {
			for (int x = 0; x < WIDTH; x++) {

				if (rand() % 2 == 0) {
					chunkData[x + y * WIDTH + z * WIDTH] = Block::Stone;
				
				} else {
					chunkData[x + y * WIDTH + z * WIDTH] = Block::Empty;
				}
			}
		}
	}
}
