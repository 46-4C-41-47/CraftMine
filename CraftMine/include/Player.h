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
	Player() : position{ glm::vec3(0.0f) }, cam{ Camera() }, chunkPos{glm::vec2(0.0f)}, previousChunkPos{glm::vec2(0.0f)} {}
	~Player();

	glm::vec2 getChunkPos();
	glm::vec2 getPreviousChunkPos();
};
