#pragma once

#include <vector>
#include <unordered_map>

#include "BufferElement.h"


typedef struct {
	int x, y, z;
} mapKey;


class FaceBuffer {
private:
	bool dirty = true;
	std::unordered_map<mapKey, face>* map = new std::unordered_map<mapKey, face>;
	std::vector<face>* packedData = new std::vector<face>;

public:
	~FaceBuffer();

	void insert(int x, int y, int z, int textureIndex);
	void remove(int x, int y, int z);
	const std::vector<face>& getData();
};
