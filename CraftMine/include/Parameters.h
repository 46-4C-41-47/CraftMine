#pragma once

static const int NORTH = 0;
static const int SOUTH = 1;
static const int EAST = 2;
static const int WEST = 3;


namespace params 
{
	namespace chunk
	{
		static const int
			WIDTH = 16,
			HEIGHT = 256,
			RADIUS = 8, // number of chunks visible around the player
			SPREAD = 4, // number of noise value on one chunk border
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
