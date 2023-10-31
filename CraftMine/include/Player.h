#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Parameters.h"


class Player {
private:
	unsigned int height = 2;
	glm::vec3 position;
	glm::vec2 previousChunkPos, chunkPos;
	Camera cam;

public:
	const unsigned int PLAYER_HEIGTH = 2;

	Player(glm::vec3 playerPosition) : position { glm::vec3(0.0f) }, chunkPos { glm::vec2(0.0f) }, previousChunkPos { glm::vec2(0.0f) } 
	{
		cam.move(playerPosition);
	}
	~Player() {}

	void updateChunkPos();
	
	glm::vec2 getChunkPos() { return chunkPos; }
	glm::vec2 getPreviousChunkPos() { return previousChunkPos; }

	Camera& getCam() { return cam; }
};
