#include "../include/FaceBuffer.h"


FaceBuffer::~FaceBuffer()
{
	delete packedData, map;
}


void FaceBuffer::insert(int x, int y, int z, int textureIndex)
{
	map->insert({ getIndex(x, y, z), {x, y, z, textureIndex}});
	dirty = true;
}


void FaceBuffer::remove(int x, int y, int z)
{
	map->erase(getIndex(x, y, z));
	dirty = true;
}


const std::vector<face>& FaceBuffer::getData()
{
	if (not dirty)
	{
		return *packedData;
	}

	for (const std::pair<long long int, face>& kv : (*map))
	{
		packedData->push_back(kv.second);
	}

	dirty = false;
	return *packedData;
}
