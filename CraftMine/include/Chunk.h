#pragma once

#include <stdlib.h>
#include <time.h>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Block.h"
#include "Mesh.h"
#include "Shader.h"


class Chunk {
private:
	const int MIN_HEIGHT = 128, MAX_HEIGHT = 192;
    unsigned int texture;
    bool needToUpdate = true;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    Mesh* mesh;
	Shader* shader;
	Light* light;
	Block::Type* chunkData;

	void init();
	void addWater();
	void createCaves();
	void generateMesh();

public:
	const int WIDTH = 16, HEIGHT = 192;

	Chunk(Light* l, unsigned int t);
	~Chunk();

    void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);
};