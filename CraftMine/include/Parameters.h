#pragma once

#include <glm/glm.hpp>

#include <future>
#include <chrono>


namespace constants
{
	enum cardinal 
	{
		NORTH = 0,
		SOUTH = 1,
		 EAST = 2,
		 WEST = 3,
	};

	enum cube 
	{
		 FRONT = 0,
		  BACK = 1,
		  LEFT = 2,
		 RIGHT = 3,
		BOTTOM = 4,
		   TOP = 5,
	};

	enum BlockType
	{
		Null = -1,
		Empty = 1023,
		Dirt = 200,
		Grass = 3,
		Stone = 4,
		Water = 5,
		Lava = 6,
		Wood = 7,
		Leaf = 8,
		Diamond = 9,
	};

	namespace texture
	{
		static const int PIXEL_WIDTH = 512;
		static const int CHUNK_PIXEL_WIDTH = 16;
		static const int CHUNK_COUNT = PIXEL_WIDTH / CHUNK_PIXEL_WIDTH;
		static const float CHUNK_WIDTH = CHUNK_PIXEL_WIDTH / (float)PIXEL_WIDTH;
	}
}

namespace params 
{
	namespace chunk
	{
		static const int
			WIDTH = 16,
			HEIGHT = 256,
			RADIUS = 10, // nombre de chunks visible autour du joueur
			SPREAD = 4,
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
			FRAME_INTERVAL = 1000000.0f / 60.0f, // en microsecondes
			ASPECT_RATIO = (float)FRAME_WIDTH / (float)FRAME_HEIGHT,
			NEAR_PLANE = 0.1f,
			FAR_PLANE = chunk::WIDTH * chunk::RADIUS;
		static const glm::vec3 
			FOG_COLOR = glm::vec3(0.0f),
			SKY_COLOR = glm::vec3(0.0f);
			//SKY_COLOR = glm::vec3(0.37f, 0.7f, 0.75f);
	}

	namespace controls
	{
		static const float HORIZONTAL_SENSITIVITY = 0.002f, VERTICAL_SENSITIVITY = 0.0016f;
		static const float CAM_SPEED = 15.0f;
	}
}


namespace func
{
	static inline float getVIndex(int position, float offset)
	{
		return (((int)(position / constants::texture::CHUNK_COUNT)) 
			/ 
			(float)constants::texture::CHUNK_COUNT)
			+ offset * constants::texture::CHUNK_WIDTH;
	}

	static inline float getUIndex(int position, float offset)
	{
		return ((position % constants::texture::CHUNK_COUNT) 
			/ 
			(float)constants::texture::CHUNK_COUNT)
			+ offset * constants::texture::CHUNK_WIDTH;
	}

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
