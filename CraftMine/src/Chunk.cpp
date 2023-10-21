#include "../include/Chunk.h"


ThreadPool* Chunk::threadPool = ThreadPool::getInstance();


Chunk::Chunk(int x, int y, Light* l, unsigned int t) : x{ x }, y{ y }, light{ l }, texture{ t }
{
	chunkData = new Block::Type[WIDTH * WIDTH * HEIGHT];
	chunkDataSize = WIDTH * WIDTH * HEIGHT;
	init();
	generateMesh();
}


Chunk::~Chunk() 
{
	delete[] chunkData;
	delete mesh, buffer;
}


void Chunk::init()
{
	Noise n(params::noise::SEED);
	const int noiseBorderSize = WIDTH * SPREAD,
			  half_height = (float)HEIGHT * 0.5f,
			  offsetX = ((SPREAD + (this->x % SPREAD)) % SPREAD) * WIDTH,
			  offsetY = ((SPREAD + (this->y % SPREAD)) % SPREAD) * WIDTH;

	double* heightMap(
		n.detailed2DNoise(
			floor((float)this->x / SPREAD) + params::noise::FREQUENCY,
			floor((float)this->y / SPREAD) + params::noise::FREQUENCY,
			noiseBorderSize
		)
	);

	for (int z = 0; z < WIDTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (y < heightMap[(x + offsetX) + (z + offsetY) * noiseBorderSize] * HEIGHT_RANGE + half_height)
					chunkData[getChunkIndex(x, y, z)] = Block::Type::Dirt;
				else
					chunkData[getChunkIndex(x, y, z)] = Block::Type::Empty;
			}
		}
	}

	delete heightMap;
}


inline Block::Type Chunk::getBlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH) && (0 <= y && y < HEIGHT) && (0 <= z && z < WIDTH))
		return chunkData[getChunkIndex(x, y, z)];
	return Block::Type::Null;
}


bool Chunk::isThereABlock(int x, int y, int z)
{
	if ((0 <= x && x < WIDTH ) &&
		(0 <= y && y < HEIGHT) &&
		(0 <= z && z < WIDTH ) &&
		chunkData[getChunkIndex(x, y, z)] != Block::Type::Empty) {
		return true;
	}
	return false;
}


void Chunk::generateMesh()
{
	std::vector<BufferVertex>* meshVAO = new std::vector<BufferVertex>();

	for (int z = 0; z < WIDTH; z++)
	{
		for (int y = 0; y < HEIGHT; y++)
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
							&& !(y == 0 && i == 4)
							&& !(z == 0 && i == 0 && (neighbors[1] == nullptr || neighbors[1]->isThereABlock(x, y, WIDTH - 1)))
							&& !(x == 0 && i == 2 && (neighbors[3] == nullptr || neighbors[3]->isThereABlock(WIDTH - 1, y, z)))
							&& !(z == (WIDTH - 1) && i == 1 && (neighbors[0] == nullptr || neighbors[0]->isThereABlock(x, y, 0)))
							&& !(x == (WIDTH - 1) && i == 3 && (neighbors[2] == nullptr || neighbors[2]->isThereABlock(0, y, z))))
						{
							// iterate over each vertex which compose a cube face
							for (int j = 0; j < ChunkMeshBuffer::cube_face_size; j += 8)
							{
								int vertex_index = i * ChunkMeshBuffer::cube_face_size + j;

								BufferVertex bv = {
									getBufferId(x, y, z, i, j / 8),
									//(getIndex(x, y, z) << 16) | (i << 8) | (j / 8),

									ChunkMeshBuffer::cube_vertices[vertex_index + 0] + (float)x,
									ChunkMeshBuffer::cube_vertices[vertex_index + 1] + (float)y,
									ChunkMeshBuffer::cube_vertices[vertex_index + 2] + (float)z,

									ChunkMeshBuffer::cube_vertices[vertex_index + 3],
									ChunkMeshBuffer::cube_vertices[vertex_index + 4],
									ChunkMeshBuffer::cube_vertices[vertex_index + 5],
									ChunkMeshBuffer::cube_vertices[vertex_index + 6],
									ChunkMeshBuffer::cube_vertices[vertex_index + 7]
								};

								meshVAO->insert(meshVAO->end(), bv);
							}
						}
					}
				}
			}
		}
	}

	delete buffer, mesh;

	buffer = new ChunkMeshBuffer(meshVAO);
	mesh = new Mesh(*meshVAO, glm::vec3(x * WIDTH, 0.0f, y * WIDTH), texture);
}


void Chunk::updateBorders(int borderIndex)
{
	/*
	chunkData[getIndex(    i, j, WIDTH)]; // north
	chunkData[getIndex(    i, j,     0)]; // south
	chunkData[getIndex(WIDTH, j,     i)]; // est
	chunkData[getIndex(    0, j,     i)]; // west
	*/

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; i < HEIGHT; j++)
		{
			chunkData[getChunkIndex(i, j, WIDTH)];

			if (neighbors[borderIndex]->isThereABlock(i, j, 0))
			{
				buffer->removeFace(getBufferId(i, j, 0, 1, 0));
			}
			else
			{
				//buffer->insertFace(getBufferId(i, j, 0, 1, 0));
			}
		}
	}
}


void Chunk::draw(Shader& shader, glm::mat4& projection, glm::mat4& view) 
{
	if (mesh != nullptr)
	{
		meshMutex.lock();
		mesh->draw(shader, *light, projection, view);
		meshMutex.unlock();
	}
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

		//threadPool->submitNoReturn([=]() { this->generateMesh(); });
		//generateMesh();
	}
}


glm::vec2 Chunk::updateChunks(
	Chunk** visibleChunks,
	Light* l,
	const glm::vec2& previousPos,
	const glm::vec3& pos,
	unsigned int t
) {
	bool newChunk = false;
	double start, end0, end1, end2;

	start = glfwGetTime();

	// init
	const int borderSize = RADIUS * 2 + 1
		,camPosX = floor(pos.x / WIDTH)
		,camPosY = floor(pos.z / WIDTH)
		,offsetX = camPosX - (int)previousPos.x
		,offsetY = camPosY - (int)previousPos.y;

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
			//threadPool->submitNoReturn([=]() {
				int chunkX = camPosX + (x - RADIUS), chunkY = camPosY + (y - RADIUS);
				visibleChunks[x + y * borderSize] = new Chunk(chunkX, chunkY, l, t);
			//});
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

		neighborBuffer[0] = (y + 1) < borderSize ? visibleChunks[x + (y + 1) * borderSize] : nullptr;
		neighborBuffer[2] = (x + 1) < borderSize ? visibleChunks[(x + 1) + y * borderSize] : nullptr;
		neighborBuffer[1] = 0 <= (y - 1) ? visibleChunks[x + (y - 1) * borderSize] : nullptr;
		neighborBuffer[3] = 0 <= (x - 1) ? visibleChunks[(x - 1) + y * borderSize] : nullptr;

		if (visibleChunks[i] != nullptr)
			visibleChunks[i]->setNeighbor(neighborBuffer);

		delete visibleChunksCopy[i];
	}

	end2 = (glfwGetTime() - start) * 1000;

	/*if (newChunk)
	{
		std::cout << "(Chunk::updateChunks) elpased time :\n";
		std::cout << "\telpased time 0 : " << end0 << " ms\n";
		std::cout << "\telpased time 1 : " << end1 << " ms\n";
		std::cout << "\telpased time 2 : " << end2 << " ms\n\n";
	}*/

	return glm::vec2((float)camPosX, (float)camPosY);
}
