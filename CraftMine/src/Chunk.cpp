#include "../include/Chunk.h"


const vector<float> Chunk::cubeVerticesVAOcnt = {
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


Chunk::Chunk(int x, int y, Light* l, unsigned int t) : x{ x }, y{ y }, light{ l }, texture{ t }
{
	chunkData = new Block::Type[WIDTH * WIDTH * HEIGHT];
	chunkDataSize = WIDTH * WIDTH * HEIGHT;
	init();
}


Chunk::~Chunk() 
{
	delete[] chunkData;
	delete mesh;
}


void Chunk::init()
{
	Noise n(noiseSeed);
	const int noiseBorderSize = WIDTH * SPREAD,
			  half_height = (float)HEIGHT * 0.5f,
			  offsetX = ((SPREAD + (this->x % SPREAD)) % SPREAD) * WIDTH,
			  offsetY = ((SPREAD + (this->y % SPREAD)) % SPREAD) * WIDTH;

	double* heightMap(
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
				if (y < heightMap[(x + offsetX) + (z + offsetY) * noiseBorderSize] * HEIGHT_RANGE + half_height)
					chunkData[getIndex(x, y, z)] = Block::Type::Dirt;
				else
					chunkData[getIndex(x, y, z)] = Block::Type::Empty;
			}
		}
	}

	delete heightMap;
}


inline Block::Type Chunk::getBlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH) && (0 <= y && y < HEIGHT) && (0 <= z && z < WIDTH))
		return chunkData[getIndex(x, y, z)];
	return Block::Type::Null;
}


bool Chunk::isThereABlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH ) &&
		(0 <= y && y < HEIGHT) &&
		(0 <= z && z < WIDTH ) &&
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
				if (getBlock(x, y, z) != Block::Type::Empty)
				{
					Block::Type nearCube[] = {
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
						if ((nearCube[i] == Block::Type::Null || nearCube[i] == Block::Type::Empty)
							&& !(y == 0           && i == 4)
							&& !(z == 0           && i == 0 && (neighbors[1] == nullptr || neighbors[1]->isThereABlock(x, y, WIDTH - 1)))
							&& !(z == (WIDTH - 1) && i == 1 && (neighbors[0] == nullptr || neighbors[0]->isThereABlock(x, y,         0)))
							&& !(x == 0           && i == 2 && (neighbors[3] == nullptr || neighbors[3]->isThereABlock(WIDTH - 1, y, z)))
							&& !(x == (WIDTH - 1) && i == 3 && (neighbors[2] == nullptr || neighbors[2]->isThereABlock(        0, y, z))))
						{
							for (int j = 0; j < cubeFaceSize; j += 8)
							{
								int triangleIndex = i * cubeFaceSize + j;

								buffer[0] = cubeVerticesVAOcnt[triangleIndex + 0] + (float)x;
								buffer[1] = cubeVerticesVAOcnt[triangleIndex + 1] + (float)y;
								buffer[2] = cubeVerticesVAOcnt[triangleIndex + 2] + (float)z;

								buffer[3] = cubeVerticesVAOcnt[triangleIndex + 3];
								buffer[4] = cubeVerticesVAOcnt[triangleIndex + 4];
								buffer[5] = cubeVerticesVAOcnt[triangleIndex + 5];
								buffer[6] = cubeVerticesVAOcnt[triangleIndex + 6];
								buffer[7] = cubeVerticesVAOcnt[triangleIndex + 7];

								meshVAO.insert(meshVAO.end(), buffer, buffer + 8);
							}
						}
					}
				}
			}
		}
	}

	delete mesh;
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


void Chunk::setNeighbor(Chunk** value)
{
	if (value[0] != neighbors[0] 
		|| value[1] != neighbors[1] 
		|| value[2] != neighbors[2] 
		|| value[3] != neighbors[3]) 
	{
		neighbors[0] = value[0];
		neighbors[1] = value[1];
		neighbors[2] = value[2];
		neighbors[3] = value[3];

		generateMesh();
	}
}


glm::vec2 Chunk::updateChunks(Chunk** visibleChunks, const glm::vec2& previousPos, const glm::vec3& pos, Light* l, unsigned int t)
{
	const int borderSize = RADIUS * 2 + 1
		,camPosX = floor(pos.x / WIDTH)
		,camPosY = floor(pos.z / WIDTH)
		,offsetX = camPosX - (int)previousPos.x
		,offsetY = camPosY - (int)previousPos.y;

	int x, y, newX, newY;

	Chunk* neighborBuffer[4];
	Chunk* visibleChunksCopy[borderSize * borderSize];
	std::future<Chunk*>* asyncChunks[borderSize * borderSize] = { nullptr };

	std::copy(visibleChunks, visibleChunks + borderSize * borderSize, visibleChunksCopy);

	for (int i = 0; i < borderSize * borderSize; i++)
	{
		x = i % borderSize;
		y = i / borderSize;

		newX = x + offsetX;
		newY = y + offsetY;

		if ((0 <= newX && newX < borderSize) && (0 <= newY && newY < borderSize) && visibleChunks[x + y * borderSize] != nullptr)
		{
			visibleChunks[x + y * borderSize] = visibleChunksCopy[newX + newY * borderSize];
			visibleChunksCopy[newX + newY * borderSize] = nullptr;
		}
		else
		{
			//visibleChunks[x + y * borderSize] = new Chunk(camPosX + (x - RADIUS), camPosY + (y - RADIUS), l, t);
			asyncChunks[i] = new std::future<Chunk*>;
			*asyncChunks[i] = std::async([=]() {
				return new Chunk(camPosX + (x - RADIUS), camPosY + (y - RADIUS), l, t);
			});
		}
	}

	for (int i = 0; i < borderSize * borderSize; i++)
	{
		x = i % borderSize;
		y = i / borderSize;

		if (asyncChunks[i] != nullptr)
			visibleChunks[x + y * borderSize] = asyncChunks[i]->get();

		neighborBuffer[0] = (y + 1) < borderSize ? visibleChunks[x + (y + 1) * borderSize] : nullptr;
		neighborBuffer[2] = (x + 1) < borderSize ? visibleChunks[(x + 1) + y * borderSize] : nullptr;
		neighborBuffer[1] = 0 <= (y - 1) ? visibleChunks[x + (y - 1) * borderSize] : nullptr;
		neighborBuffer[3] = 0 <= (x - 1) ? visibleChunks[(x - 1) + y * borderSize] : nullptr;

		visibleChunks[i]->setNeighbor(neighborBuffer);

		delete visibleChunksCopy[i];
	}

	return glm::vec2((float)camPosX, (float)camPosY);
}
