#include "../include/Chunk.h"


//ThreadPool* Chunk::threadPool = ThreadPool::getInstance();


Chunk::Chunk(int x, int y, Light& l, unsigned int t) : x{ x }, y{ y }, light{ l }, texture{ t }
{
	chunkData = new BlockType[WIDTH * WIDTH * HEIGHT];
	chunkDataSize = WIDTH * WIDTH * HEIGHT;
	
	asyncBuffer = std::async([&]() {
			init();
			return generateMesh();
		});
}


Chunk::~Chunk() 
{
	delete[] chunkData;
	delete mesh, buffer;
}


void Chunk::init()
{
	Noise* n = Noise::getInstance();

	const int noiseBorderSize = WIDTH * SPREAD,
			  half_height = (float)HEIGHT * 0.5f,
			  offsetX = ((SPREAD + (this->x % SPREAD)) % SPREAD) * WIDTH,
			  offsetY = ((SPREAD + (this->y % SPREAD)) % SPREAD) * WIDTH;

	double* heightMap = n->detailed2DNoise(
		floor((float)this->x / SPREAD) + params::noise::FREQUENCY,
		floor((float)this->y / SPREAD) + params::noise::FREQUENCY,
		noiseBorderSize
	);

	for (int z = 0; z < WIDTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (y < heightMap[(x + offsetX) + (z + offsetY) * noiseBorderSize] * HEIGHT_RANGE + half_height)
					chunkData[getChunkIndex(x, y, z)] = BlockType::Dirt;
				else
					chunkData[getChunkIndex(x, y, z)] = BlockType::Empty;
			}
		}
	}

	initStatus = true;
	delete heightMap;
}


inline BlockType Chunk::getBlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH) && (0 <= y && y < HEIGHT) && (0 <= z && z < WIDTH))
		return chunkData[getChunkIndex(x, y, z)];
	return BlockType::Null;
}


bool Chunk::isThereABlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH ) &&
		(0 <= y && y < HEIGHT) &&
		(0 <= z && z < WIDTH ) &&
		chunkData[getChunkIndex(x, y, z)] != BlockType::Empty) {
		return true;
	}
	return false;
}


ChunkMeshBuffer* Chunk::generateMesh()
{
	std::vector<BufferVertex>* meshVAO = new std::vector<BufferVertex>();

	for (int z = 0; z < WIDTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (getBlock(x, y, z) != BlockType::Empty)
				{
					BlockType nearCube[] = {
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
						if ((nearCube[i] == BlockType::Null || nearCube[i] == BlockType::Empty)
							&& !(i == 4 && y == 0)
							&& !(i == 0 && z == 0)
							&& !(i == 2 && x == 0)
							&& !(i == 1 && z == (WIDTH - 1))
							&& !(i == 3 && x == (WIDTH - 1)))
						{
							// iterate over each vertex which compose a cube face
							for (int j = 0; j < ChunkMeshBuffer::cube_face_size / 8; j++)
							{
								int vertex_index = i * ChunkMeshBuffer::cube_face_size + j * 8;
								
								BufferVertex bv = {
									getBufferId(x, y, z, i, j),

									ChunkMeshBuffer::cube_vertices[vertex_index + 0] + (float)x,
									ChunkMeshBuffer::cube_vertices[vertex_index + 1] + (float)y,
									ChunkMeshBuffer::cube_vertices[vertex_index + 2] + (float)z,

									ChunkMeshBuffer::cube_vertices[vertex_index + 3],
									ChunkMeshBuffer::cube_vertices[vertex_index + 4],
									ChunkMeshBuffer::cube_vertices[vertex_index + 5],

									getUIndex(BlockType::Dirt, ChunkMeshBuffer::cube_vertices[vertex_index + 6]),
									getVIndex(BlockType::Dirt, ChunkMeshBuffer::cube_vertices[vertex_index + 7])
								};

								meshVAO->insert(meshVAO->end(), bv);
							}
						}
					}
				}
			}
		}
	}

	delete buffer;
	return new ChunkMeshBuffer(meshVAO);
}


void Chunk::updateBorders()
{
	if (buffer == nullptr)
		return;

	bordersFullyUpdated =
		   (neighbors[NORTH] == nullptr || neighbors[NORTH]->getInitStatus())
		&& (neighbors[SOUTH] == nullptr || neighbors[SOUTH]->getInitStatus())
		&& (neighbors[EAST ] == nullptr || neighbors[EAST ]->getInitStatus())
		&& (neighbors[WEST ] == nullptr || neighbors[WEST ]->getInitStatus());

	bufferMutex.lock();
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			// NORTH
			if (neighbors[NORTH] != nullptr 
				&& !neighbors[NORTH]->isThereABlock(i, j, 0)
				&& isThereABlock(i, j, params::chunk::WIDTH - 1))
			{
				buffer->insertFace(i, j, params::chunk::WIDTH - 1, BACK, BlockType::Dirt);
			}

			// SOUTH
			if (neighbors[SOUTH] != nullptr
				&& !neighbors[SOUTH]->isThereABlock(i, j, params::chunk::WIDTH - 1)
				&& isThereABlock(i, j, 0))
			{
				buffer->insertFace(i, j, 0, FRONT, BlockType::Dirt);
			}

			// EAST
			if (neighbors[EAST] != nullptr
				&& !neighbors[EAST]->isThereABlock(0, j, i)
				&& isThereABlock(params::chunk::WIDTH - 1, j, i))
			{
				buffer->insertFace(params::chunk::WIDTH - 1, j, i, RIGHT, BlockType::Dirt);
			}

			// WEST
			if (neighbors[WEST] != nullptr
				&& !neighbors[WEST]->isThereABlock(params::chunk::WIDTH - 1, j, i)
				&& isThereABlock(0, j, i))
			{
				buffer->insertFace(0, j, i, LEFT, BlockType::Dirt);
			}
		}
	}
	bufferMutex.unlock();
	
	updateMesh();
}


void Chunk::updateMesh()
{
	meshMutex.lock();
	delete mesh;
	mesh = new Mesh(*buffer->getData(), glm::vec3(x * WIDTH, 0.0f, y * WIDTH), texture);
	meshMutex.unlock();
}


void Chunk::draw(Shader& shader, glm::mat4& projection, glm::mat4& view) 
{
	if (mesh == nullptr)
	{
		if (func::isFuturReady(asyncBuffer))
		{
			bufferMutex.lock();
			buffer = asyncBuffer.get();
			bufferMutex.unlock();
			updateMesh();
		}
		else
		{
			return;
		}
	}

	meshMutex.lock();
	mesh->draw(shader, light, projection, view);
	meshMutex.unlock();
}


void Chunk::setNeighbor(Chunk** value)
{
	if (   value[NORTH] != neighbors[NORTH]
		|| value[SOUTH] != neighbors[SOUTH]
		|| value[EAST ] != neighbors[EAST ]
		|| value[WEST ] != neighbors[WEST ]
		|| !bordersFullyUpdated)
	{
		neighbors[NORTH] = value[NORTH];
		neighbors[SOUTH] = value[SOUTH];
		neighbors[EAST ] = value[EAST ];
		neighbors[WEST ] = value[WEST ];

		bordersFullyUpdated = false;
		//std::thread(&Chunk::updateBorders, this).detach();
		updateBorders();
	}
}


void Chunk::updateChunks(
	Chunk** visibleChunks,
	Light& l,
	Player& p,
	unsigned int t
) {
	bool newChunk = false;
	double start, end0, end1, end2;

	start = glfwGetTime();

	p.updateChunkPos();
	glm::vec2 camPos = p.getChunkPos();
	glm::vec2 previousPos = p.getPreviousChunkPos();

	const int borderSize = RADIUS * 2 + 1
		, offsetX = camPos.x - (int)previousPos.x
		, offsetY = camPos.y - (int)previousPos.y;

	int x, y, newX, newY;

	Chunk* neighborBuffer[4];
	Chunk* visibleChunksCopy[borderSize * borderSize];

	std::copy(visibleChunks, visibleChunks + borderSize * borderSize, visibleChunksCopy);

	end0 = (glfwGetTime() - start) * 1000;
	start = glfwGetTime();

	// phase 1
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
			int chunkX = camPos.x + (x - RADIUS), chunkY = camPos.y + (y - RADIUS);
			visibleChunks[x + y * borderSize] = new Chunk(chunkX, chunkY, l, t);
			newChunk = true;
		}
	}

	end1 = (glfwGetTime() - start) * 1000;
	start = glfwGetTime();

	// phase 2
	for (int i = 0; i < borderSize * borderSize; i++)
	{
		x = i % borderSize;
		y = i / borderSize;

		neighborBuffer[NORTH] = (y + 1) < borderSize ? visibleChunks[x + (y + 1) * borderSize] : nullptr;
		neighborBuffer[EAST ] = (x + 1) < borderSize ? visibleChunks[(x + 1) + y * borderSize] : nullptr;
		neighborBuffer[SOUTH] = 0 <= (y - 1) ? visibleChunks[x + (y - 1) * borderSize] : nullptr;
		neighborBuffer[WEST ] = 0 <= (x - 1) ? visibleChunks[(x - 1) + y * borderSize] : nullptr;

		visibleChunks[i]->setNeighbor(neighborBuffer);

		delete visibleChunksCopy[i];
	}

	end2 = (glfwGetTime() - start) * 1000;

	/*if (newChunk)
	{
		std::cout << "(Chunk::updateChunks) elpased time :\n";
		std::cout << "\telpased time 0 : " << end0 << " ms\n";
		std::cout << "\telpased time 1 : " << end1 << " ms\n";
		std::cout << "\telpased time 2 : " << end2 << " ms\n";
		std::cout << "\ttotal : " << (end0 + end1 + end2) << " ms\n\n";
	}*/
}
