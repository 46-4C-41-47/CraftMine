#include "../include/ChunkMeshMap.h"


ChunkMeshMap::~ChunkMeshMap()
{
	delete map, data;
}


void ChunkMeshMap::insert(int x, int y, int z, float u, float v)
{
	needToUpdate = true;

	int id = (x << 16) | (y << 8) | z;
	map->insert({ id, { (float)x, (float)y, (float)z, u, v } });
}


std::vector<Vertex>& ChunkMeshMap::getData()
{
	if (data != nullptr and not needToUpdate)
		return *data;

	needToUpdate = false;
	delete data;
	data = new std::vector<Vertex>(map->size());

	for (const std::pair<const int, Vertex>& key : *map)
		data->push_back(key.second);

	return *data;
}
