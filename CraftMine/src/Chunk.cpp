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
	Noise n(6817.8643);
	double frequency = 50.461;
	std::unique_ptr<double> heightMap(n.detailedNoise(x + frequency, y + frequency, WIDTH));

	int half_height = (float)HEIGHT * 0.5f;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int z = 0; z < WIDTH; z++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (y < heightMap.get()[x + z * WIDTH] * HEIGHT_RANGE + half_height)
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

					for (int i = 0; i < 6; i++)
					{
						if (nearCube[i] == nullptr || *nearCube[i] == Block::Type::Empty)
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
