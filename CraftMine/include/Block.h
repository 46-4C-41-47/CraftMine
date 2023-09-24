#pragma once

#include <string>


namespace Block
{
	enum class Type 
	{
		Empty = 0,
		Dirt = 1,
		Grass = 2,
		Stone = 3,
		Water = 4,
		Lava = 5,
		Wood = 6,
		Leaf = 7,
		Diamond = 8,
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
