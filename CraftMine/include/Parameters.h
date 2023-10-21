#pragma once


enum class cardinal
{
	NORTH = 0,
	SOUTH = 1,
	 EAST = 2,
	 WEST = 3
};


namespace params 
{
	namespace chunk
	{
		static const int
			WIDTH = 16,
			HEIGHT = 256,
			RADIUS = 8,
			SPREAD = 4,
			GROUND_MIN_HEIGHT = 128,
			GROUND_MAX_HEIGHT = 192,
			HEIGHT_RANGE = GROUND_MAX_HEIGHT - GROUND_MIN_HEIGHT;
	}
	
	namespace noise
	{
		static const double FREQUENCY = 0.15413, SEED = 684.6565;
	}
}


namespace func
{
	static inline int getChunkIndex(int x, int y, int z) 
	{ 
		return x + (y * params::chunk::WIDTH) + (z * params::chunk::WIDTH * params::chunk::HEIGHT);
	}
	
	static inline unsigned int getBufferId(int x, int y, int z, int faceIndex, int vertexIndex)
	{
		return (getChunkIndex(x, y, z) << 16) | (faceIndex << 8) | vertexIndex;
	}
}


namespace data
{

}
