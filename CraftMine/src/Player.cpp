#include "../include/Player.h"


glm::vec2 Player::getChunkPos()
{
	previousChunkPos = chunkPos;
	chunkPos = glm::vec2(floor(position.x / params::chunk::WIDTH), floor(position.z / params::chunk::WIDTH));
	return chunkPos;
}


glm::vec2 Player::getPreviousChunkPos() { return previousChunkPos; }
