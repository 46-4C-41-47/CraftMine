#include "../include/Player.h"


void Player::updateChunkPos()
{
	previousChunkPos = chunkPos;
	chunkPos = glm::vec2(floor(cam.position.x / params::chunk::WIDTH), floor(cam.position.z / params::chunk::WIDTH));
}
