#pragma once

#include "BufferElement.h"

#include <map>
#include <vector>


class ChunkMeshMap
{
private:
	bool needToUpdate = true;
	std::map<int, Vertex>* map;
	std::vector<Vertex>* data = nullptr;

public:
	ChunkMeshMap() : map{ new std::map<int, Vertex> } { }
	ChunkMeshMap(std::map<int, Vertex>* src) : map{ src } { }

	~ChunkMeshMap();

	void insert(int x, int y, int z, float u, float v);

	std::vector<Vertex>& getData();
};
