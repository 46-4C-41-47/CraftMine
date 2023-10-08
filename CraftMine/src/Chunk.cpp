#include "../include/Chunk.h"


namespace Cube
{
	const int faceSize = 48;

	// { front 0, back 1, left 2, right 3, bottom 4, top 5 }
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


Chunk::Chunk(int x, int y, Light* l, unsigned int t) : x{ x }, y{ y }, light{ l }, texture{ t }
{
	chunkData = new Block::Type[WIDTH * WIDTH * HEIGHT];
	chunkDataSize = WIDTH * WIDTH * HEIGHT;
	init();
}


Chunk::~Chunk() 
{
	delete mesh, blocks;
}


void Chunk::init()
{
	Noise n(noiseSeed);
	const int noiseBorderSize = WIDTH * SPREAD,
			  half_height = (float)HEIGHT * 0.5f,
			  offsetX = ((SPREAD + (this->x % SPREAD)) % SPREAD) * WIDTH,
			  offsetY = ((SPREAD + (this->y % SPREAD)) % SPREAD) * WIDTH;

	std::unique_ptr<double> heightMap(
		n.detailed2DNoise(
			floor((float)this->x / SPREAD) + noiseFrequency,
			floor((float)this->y / SPREAD) + noiseFrequency,
			noiseBorderSize
		)
	);

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int z = 0; z < WIDTH; z++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (y < heightMap.get()[(x + offsetX) + (z + offsetY) * noiseBorderSize] * HEIGHT_RANGE + half_height)
					chunkData[getIndex(x, y, z)] = Block::Type::Dirt;
				else
					chunkData[getIndex(x, y, z)] = Block::Type::Empty;
			}
		}
	}
}


inline Block::Type* Chunk::getBlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH) && (0 <= y && y < HEIGHT) && (0 <= z && z < WIDTH))
		return &(chunkData[getIndex(x, y, z)]);
	return nullptr;
}


bool Chunk::isThereABlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH) &&
		(0 <= y && y < HEIGHT) &&
		(0 <= z && z < WIDTH) &&
		chunkData[getIndex(x, y, z)] != Block::Type::Empty) {
		return true;
	}
	return false;
}


void Chunk::generateMesh()
{
	vector<float> meshVAO;
	float buffer[8];

	// Un algo d'une complexité t'as peur 
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int z = 0; z < WIDTH; z++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (*getBlock(x, y, z) != Block::Type::Empty)
				{
					Block::Type* nearCube[] = {
						getBlock(x    , y    , z - 1),
						getBlock(x    , y    , z + 1),
						getBlock(x - 1, y    , z    ),
						getBlock(x + 1, y    , z    ),
						getBlock(x    , y - 1, z    ),
						getBlock(x    , y + 1, z    ),
					};

					// iterate over each cube face and create the mesh buffer
					for (int i = 0; i < 6; i++)
					{
						if ((nearCube[i] == nullptr || *nearCube[i] == Block::Type::Empty) 
							&& !(z == 0           && i == 0 && (neighbors[1] == nullptr || !neighbors[1]->isThereABlock(x, y, WIDTH - 1)))
							&& !(z == (WIDTH - 1) && i == 1 && (neighbors[0] == nullptr || !neighbors[0]->isThereABlock(x, y,         0)))
							&& !(x == 0           && i == 2 && (neighbors[3] == nullptr || !neighbors[3]->isThereABlock(WIDTH - 1, y, z)))
							&& !(x == (WIDTH - 1) && i == 3 && (neighbors[2] == nullptr || !neighbors[2]->isThereABlock(        0, y, z))))
						{
							for (int j = 0; j < Cube::faceSize; j += 8)
							{
								int triangleIndex = i * Cube::faceSize + j;

								buffer[0] = Cube::verticesVAOcnt[triangleIndex + 0] + (float)x;
								buffer[1] = Cube::verticesVAOcnt[triangleIndex + 1] + (float)y;
								buffer[2] = Cube::verticesVAOcnt[triangleIndex + 2] + (float)z;

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
	}

	mesh = new Mesh(meshVAO, glm::vec3(x * WIDTH, 0.0f, y * WIDTH), texture);

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


void Chunk::setNeighbor(int index, Chunk* value)
{
	neighbors[index] = value;

	delete mesh;
	generateMesh();
}


glm::vec2 Chunk::updateChunks(Chunk** visibleChunks, const glm::vec2& previousPos, const glm::vec3& pos, Light* l, unsigned int t)
{
	const int borderSize = RADIUS * 2 + 1;
	
	int camPosX = floor(pos.x / WIDTH);
	int camPosY = floor(pos.z / WIDTH);

	int offsetX = camPosX - (int)previousPos.x;
	int offsetY = camPosY - (int)previousPos.y;

	int newX, newY;

	Chunk* visibleChunksCopy[borderSize * borderSize];

	std::copy(visibleChunks, visibleChunks + borderSize * borderSize, visibleChunksCopy);

	for (int x = 0; x < borderSize; x++)
	{
		for (int y = 0; y < borderSize; y++)
		{
			newX = x + offsetX;
			newY = y + offsetY;

			if ((0 <= newX && newX < borderSize) && (0 <= newY && newY < borderSize) && visibleChunks[x + y * borderSize] != nullptr)
				visibleChunks[x + y * borderSize] = visibleChunksCopy[newX + newY * borderSize];
			else 
				visibleChunks[x + y * borderSize] = new Chunk(camPosX + (x - RADIUS), camPosY + (y - RADIUS), l, t);

			/*// set current chunk north pointer
			if (0 <= y - 1)
			{
				visibleChunks[x + y * borderSize]->setNeighbor(0, visibleChunks[x + (y - 1) * borderSize]);
				visibleChunks[x + (y - 1) * borderSize]->setNeighbor(2, visibleChunks[x + y * borderSize]);
			}
			else
			{
				visibleChunks[x + y * borderSize]->setNeighbor(0, nullptr);
			}

			// set current chunk west pointer
			if (0 <= x - 1)
			{
				visibleChunks[x + y * borderSize]->setNeighbor(3, visibleChunks[(x - 1) + y * borderSize]);
				visibleChunks[(x - 1) + y * borderSize]->setNeighbor(2, visibleChunks[x + y * borderSize]);
			}
			else
			{
				visibleChunks[x + y * borderSize]->setNeighbor(3, nullptr);
			}*/
		}
	}

	return glm::vec2((float)camPosX, (float)camPosY);
}
