#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Parameters.h"
#include "BufferElement.h"


class ChunkMeshBuffer {
private:
	std::vector<BufferVertex>* buffer;

	int getInterpolationIndex(unsigned int id);

public:
	static const std::vector<float> cube_vertices;
	static const int VERTEX_SIZE = 8, COORDINATES_SIZE = 3, NORMALS_SIZE = 3, TEXTURES_SIZE = 2;
	static const int cube_face_size = 48;

	ChunkMeshBuffer() : buffer{ new std::vector<BufferVertex> } {}
	ChunkMeshBuffer(std::vector<BufferVertex>* constructedBuffer);
	~ChunkMeshBuffer();

	int find(unsigned int id);

	void insertCube(int x, int y, int z);
	void removeCube(unsigned int id);

	void insertFace(int x, int y, int z, int face_index);
	void removeFace(unsigned int id);

	void print();

	std::vector<BufferVertex>* getData() { return buffer; }
};
