#pragma once

#include <vector>
#include <unordered_map>

#include "Parameters.h"
#include "BufferElement.h"


class FaceBuffer {
private:
	bool dirty = true;
	std::unordered_map<long long int, face>* map = new std::unordered_map<long long int, face>;
	std::vector<face>* packedData = new std::vector<face>;

	inline long long int getIndex(int x, int y, int z) 
	{ return ((long long int)x << 42) | ((long long int)y << 21) | (long long int)z; }

public:
	const int faceType;

	FaceBuffer(int ft) : faceType{ ft } {}
	~FaceBuffer();

	void insert(int x, int y, int z, int textureIndex);
	void remove(int x, int y, int z);
	const std::vector<face>& getData();
};
