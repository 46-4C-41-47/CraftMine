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
	//vector<float> meshVAO = {};
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
					/*meshVAO.insert(meshVAO.begin(), Block::Cube::front.begin(), Block::Cube::front.end());
					meshVAO.insert(meshVAO.begin(), Block::Cube::back.begin(), Block::Cube::back.end());
					meshVAO.insert(meshVAO.begin(), Block::Cube::left.begin(), Block::Cube::left.end());
					meshVAO.insert(meshVAO.begin(), Block::Cube::right.begin(), Block::Cube::right.end());
					meshVAO.insert(meshVAO.begin(), Block::Cube::bottom.begin(), Block::Cube::bottom.end());
					meshVAO.insert(meshVAO.begin(), Block::Cube::top.begin(), Block::Cube::top.end());*/
				}
			}
		}
	}

	//test = new Mesh(meshVAO, glm::vec3(0.0f, 0.0f, 0.0f), texture);

	needToUpdate = false;
}


void Chunk::draw(Shader& shader, glm::mat4& projection, glm::mat4& view) 
{
	if (needToUpdate)
	{
		generateMesh();
	}

	for (int i = 0; i < mesh->size(); i++)
	{
		(*mesh)[i].draw(shader, *light, projection, view);
	}

	//test->draw(shader, *light, projection, view);
}


vector<Mesh> Chunk::getMesh() 
{
	if (needToUpdate) 
	{
		generateMesh();
	}

	return *mesh;
}
