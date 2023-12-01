#include "../include/FaceBuffer.h"


FaceBuffer::~FaceBuffer()
{
	delete packedData, map;
}


void FaceBuffer::insert(int x, int y, int z, int textureIndex)
{
	map->insert({ { x, y, z }, { x, y, z, textureIndex } });
}


void FaceBuffer::remove(int x, int y, int z)
{
	map->erase({ x, y, z });
}


const std::vector<face>& FaceBuffer::getData()
{
	if (not dirty)
	{
		return *packedData;
	}

	for (const std::pair<mapKey, face>& kv : (*map))
	{
		packedData->push_back(kv.second);
	}

	dirty = false;
	return *packedData;
}
