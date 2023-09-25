#include "../include/Chunk.h"


Chunk::Chunk(Light* l, unsigned int t) 
{
	chunkData = new Block::Type[WIDTH * WIDTH * HEIGHT];
	light = l;
	texture = t;

	init();
}


Chunk::~Chunk() 
{
	delete[] chunkData;
	delete mesh;
}


void Chunk::init() 
{
	srand(time(NULL));

	for (int z = 0; z < HEIGHT; z++) 
	{
		for (int y = 0; y < WIDTH; y++) 
		{
			for (int x = 0; x < WIDTH; x++) 
			{
				if (rand() % 2 == 0) 
				{
					chunkData[x + y * WIDTH + z * WIDTH] = Block::Type::Stone;
				} 
				else 
				{
					chunkData[x + y * WIDTH + z * WIDTH] = Block::Type::Empty;
				}
			}
		}
	}
}


void Chunk::generateMesh() 
{
	mesh = new vector<Mesh>();

	for (int z = 0; z < HEIGHT; z++)
	{
		for (int y = 0; y < WIDTH; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (chunkData[x + y * WIDTH + z * WIDTH] != Block::Type::Empty)
				{
					mesh->push_back(Mesh(cubeVAO, glm::vec3(x + position.x, z + position.z, y + position.y), texture));
				}
			}
		}
	}

	needToUpdate = false;
}


vector<Mesh> Chunk::getMesh() 
{
	if (needToUpdate) 
	{
		generateMesh();
	}

	return *mesh;
}
