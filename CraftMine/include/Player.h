#pragma once

#include <glm/glm.hpp>


class Player {
private:
	unsigned int height;
	glm::vec2 position;

public:
	Player();
	~Player();
};
