#pragma once

static const int BV_ELEMENT_COUNT = 9;
static const int BV_DISPOSITION_SIZE = 4;
static const int BV_DISPOSITION[4] = { 1, 3, 3, 2 };


typedef struct {
	// l'id est un entier sur 32 bit
	// les 16 premiers bits sont les coordonnées dans le chunk
	// les 8 suivants l'index de la face 
	// et les 8 derniers l'index du point
	unsigned int id;

	float x;
	float y;
	float z;
	
	float normal_x;
	float normal_y;
	float normal_z;

	float texture_x;
	float texture_y;
} BufferVertex;



typedef struct {
	int x;
	int y;
	int z;

	int textureIndex;
} face;
