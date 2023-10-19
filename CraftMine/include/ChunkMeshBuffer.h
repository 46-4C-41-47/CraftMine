#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "BufferElement.h"


class ChunkMeshBuffer {
private:
	static const int cube_face_size = 48;
	static const std::vector<float> cube_vertices;
	std::vector<BufferVertex>* buffer;

	int getInterpolationIndex(int id);

public:
	static const int VERTEX_SIZE = 8, COORDINATES_SIZE = 3, NORMALS_SIZE = 3, TEXTURES_SIZE = 2;

	ChunkMeshBuffer() : buffer{ new std::vector<BufferVertex> } {}
	ChunkMeshBuffer(std::vector<BufferVertex>* constructedBuffer);
	~ChunkMeshBuffer();

	void insertCube(int id, int x, int y, int z);
	void removeCube(int id);

	void insertFace(int id, int x, int y, int z, int face_index);
	void removeFace(int id, int x, int y, int z, int face_index);

	void print();

	std::vector<BufferVertex>* getData() { return buffer; }
};
