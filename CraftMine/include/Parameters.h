#pragma once

#include <future>
#include <chrono>


namespace constants
{
	static const int NORTH = 0;
	static const int SOUTH = 1;
	static const int  EAST = 2;
	static const int  WEST = 3;

	static const int  FRONT = 0;
	static const int   BACK = 1;
	static const int   LEFT = 2;
	static const int  RIGHT = 3;
	static const int BOTTOM = 4;
	static const int    TOP = 5;
}

namespace params 
{
	namespace chunk
	{
		static const int
			WIDTH = 16,
			HEIGHT = 256,
			RADIUS = 20, // nombre de chunks visible autour du joueur
			SPREAD = 8,
			GROUND_MIN_HEIGHT = 128,
			GROUND_MAX_HEIGHT = 192,
			HEIGHT_RANGE = GROUND_MAX_HEIGHT - GROUND_MIN_HEIGHT;
	}
	
	namespace noise
	{
		static const double 
			FREQUENCY = 0.15413, 
			SEED = 684.6565;
	}

	namespace graphical
	{
		static const int FRAME_WIDTH = 1200, FRAME_HEIGHT = 800;
		static const float 
			DELTA = 1000.0f / 60, 
			ASPECT_RATIO = (float)FRAME_WIDTH / (float)FRAME_HEIGHT,
			NEAR_PLANE = 0.1f,
			FAR_PLANE = 400.0f;
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

	template<typename T>
	static inline bool isFuturReady(const std::future<T>& f) 
	{
		return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}
}


namespace data
{

}
