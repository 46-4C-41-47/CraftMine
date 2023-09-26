#include "../include/Chunk.h"


namespace Cube
{
	const int faceSize = 48;

	const vector<float> verticesVAOcnt = {
		// vertex coordinates    normal vector         texture coordinates
		  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // front
		   0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		   0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		   0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		  -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		  -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // back
		   0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

		  -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // left
		  -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		  -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		  -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		  -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		   0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // right
		   0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		   0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		   0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		   0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		  -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // bottom
		   0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		   0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		   0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		  -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		  -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

		  -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // top
		   0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		  -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
		  -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
	};
};


bool comparator(Block::ChunkBlock a, Block::ChunkBlock b) { return a.id < b.id; }


Chunk::Chunk(Light* l, unsigned int t) 
{
	chunkData = new Block::Type[WIDTH * WIDTH * HEIGHT];
	light = l;
	texture = t;

	init();
}


Chunk::~Chunk() 
{
	delete mesh, blocks;
}

/*
void Chunk::init() 
{
	blocks = new std::vector<Block::ChunkBlock>();
	srand(time(NULL));

	for (int z = 0; z < HEIGHT; z++) 
	{
		for (int y = 0; y < WIDTH; y++) 
		{
			for (int x = 0; x < WIDTH; x++) 
			{
				Block::ChunkBlock block = { (x + y * WIDTH + z * WIDTH), x, y, z, Block::Type::Dirt };
				blocks->push_back(block);
			}
		}
	}

	std::sort(blocks->begin(), blocks->end(), comparator);
}


void Chunk::generateMesh() 
{
	vector<float> meshVAO;
	float buffer[8];

	for (int i = 0; i < blocks->size(); i++)
	{
		for (int j = 0; j < 6; j++) 
		{
			for (int k = 0; k < Cube::faceSize; k += 8)
			{
				int triangleIndex = j * Cube::faceSize + k;

				// z and y axis get swaped to make z the up/down axis
				buffer[0] = Cube::verticesVAOcnt[triangleIndex + 0] + (*blocks)[i].x;
				buffer[1] = Cube::verticesVAOcnt[triangleIndex + 1] + (*blocks)[i].z;
				buffer[2] = Cube::verticesVAOcnt[triangleIndex + 2] + (*blocks)[i].y;

				buffer[3] = Cube::verticesVAOcnt[triangleIndex + 3];
				buffer[4] = Cube::verticesVAOcnt[triangleIndex + 4];
				buffer[5] = Cube::verticesVAOcnt[triangleIndex + 5];
				buffer[6] = Cube::verticesVAOcnt[triangleIndex + 6];
				buffer[7] = Cube::verticesVAOcnt[triangleIndex + 7];

				meshVAO.insert(meshVAO.end(), buffer, buffer + 8);
			}
		}
	}

	mesh = new Mesh(meshVAO, glm::vec3(0.0f, 0.0f, 0.0f), texture);

	needToUpdate = false;
}*/


void Chunk::init()
{
	for (int z = 0; z < HEIGHT; z++)
	{
		for (int y = 0; y < WIDTH; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				chunkData[x + y * WIDTH + z * WIDTH] = Block::Type::Dirt;
			}
		}
	}
}


void Chunk::generateMesh()
{
	vector<float> meshVAO;
	float buffer[8];

	for (int z = 0; z < HEIGHT; z++)
	{
		for (int y = 0; y < WIDTH; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (chunkData[x + y * WIDTH + z * WIDTH] != Block::Type::Empty)
				{
					for (int i = 0; i < 6; i++)
					{
						for (int j = 0; j < Cube::faceSize; j += 8)
						{
							int triangleIndex = i * Cube::faceSize + j;

							// z and y axis get swaped to make z the up/down axis
							buffer[0] = Cube::verticesVAOcnt[triangleIndex + 0] + (float)x;
							buffer[1] = Cube::verticesVAOcnt[triangleIndex + 1] + (float)z;
							buffer[2] = Cube::verticesVAOcnt[triangleIndex + 2] + (float)y;

							buffer[3] = Cube::verticesVAOcnt[triangleIndex + 3];
							buffer[4] = Cube::verticesVAOcnt[triangleIndex + 4];
							buffer[5] = Cube::verticesVAOcnt[triangleIndex + 5];
							buffer[6] = Cube::verticesVAOcnt[triangleIndex + 6];
							buffer[7] = Cube::verticesVAOcnt[triangleIndex + 7];

							meshVAO.insert(meshVAO.end(), buffer, buffer + 8);
						}
					}
				}
			}
		}
	}

	mesh = new Mesh(meshVAO, glm::vec3(0.0f, 0.0f, 0.0f), texture);

	needToUpdate = false;
}


void Chunk::draw(Shader& shader, glm::mat4& projection, glm::mat4& view) 
{
	if (needToUpdate)
	{
		generateMesh();
	}

	mesh->draw(shader, *light, projection, view);
}
