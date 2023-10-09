#pragma once

#include <string>
#include <vector>


namespace Block
{
	enum class Type
	{
		Null = 0,
		Empty = 1,
		Dirt = 2,
		Grass = 3,
		Stone = 4,
		Water = 5,
		Lava = 6,
		Wood = 7,
		Leaf = 8,
		Diamond = 9,
	};


	struct ChunkBlock {
		unsigned int id, x, y, z;
		Block::Type type;
	};


	class Texture 
	{
	private:
		const char* dirtPath = ".res/textures/dirt.jpg";
		const char* grassPath = ".res/textures/grass.jpg";
		const char* stonePath = ".res/textures/stone.jpg";
		const char* waterPath = ".res/textures/water.jpg";
		const char* lavaPath = ".res/textures/lava.jpg";
		const char* woodPath = ".res/textures/wood.jpg";
		const char* leafPath = ".res/textures/leaf.jpg";
		const char* diamondPath = ".res/textures/diamond.jpg";

		static unsigned int dirt, grass, stone, water, lava, wood, leaf, diamond;

		void loadTexture(std::string);

	public:
		Texture();

		unsigned int getDirt();
		unsigned int getGrass();
		unsigned int getStone();
		unsigned int getWater();
		unsigned int getLava();
		unsigned int getWood();
		unsigned int getLeaf();
		unsigned int getDiamond();
	};
}

