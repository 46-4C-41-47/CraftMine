#include "../include/ChunkMeshBuffer.h"


const std::vector<float> ChunkMeshBuffer::cube_vertices = {
	// vertex coordinates    normal vector         texture coordinates
	  0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f, // front
	  1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
	  1.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
	  1.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
	  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
	  0.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

	  0.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f, // back
	  1.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
	  1.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
	  1.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
	  0.0f, 1.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
	  0.0f, 0.0f, 1.0f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

	  0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // left
	  0.0f, 1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	  0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	  0.0f, 0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	  0.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	  0.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

	  1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f, // right
	  1.0f, 1.0f, 0.0f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
	  1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	  1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	  1.0f, 0.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	  1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

	  0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // bottom
	  1.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
	  1.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	  1.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
	  0.0f, 0.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	  0.0f, 0.0f, 0.0f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

	  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f, // top
	  1.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
	  1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	  1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
	  0.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
};


ChunkMeshBuffer::ChunkMeshBuffer(std::vector<BufferVertex>* constructedBuffer) : buffer{ constructedBuffer } 
{
	std::sort(buffer->begin(), buffer->end(), 
		[](const BufferVertex& a, const BufferVertex& b) { return a.id < b.id; }
	);
}


ChunkMeshBuffer::~ChunkMeshBuffer()
{
	delete buffer;
}


int ChunkMeshBuffer::getInterpolationIndex(int id)
{
	for (int i = 0; i < buffer->size(); i++)
	{
		if (id < (*buffer)[i].id)
		{
			return i;
		}
	}
}


void ChunkMeshBuffer::insertCube(int id, int x, int y, int z)
{

}


void ChunkMeshBuffer::insertFace(int id, int x, int y, int z, int face_index)
{
	int index = getInterpolationIndex(id);

	for (int i = 0; i < cube_face_size; i += 8)
	{
		int vertex_index = face_index * cube_face_size + i;

		BufferVertex bv = {
			id,

			cube_vertices[vertex_index + 0] + (float)x,
			cube_vertices[vertex_index + 1] + (float)y,
			cube_vertices[vertex_index + 2] + (float)z,

			cube_vertices[vertex_index + 3],
			cube_vertices[vertex_index + 4],
			cube_vertices[vertex_index + 5],
			cube_vertices[vertex_index + 6],
			cube_vertices[vertex_index + 7]
		};

		buffer->insert(buffer->begin() + index + i / 8, bv);
	}
}


void ChunkMeshBuffer::print()
{
	for (int i = 0; i < buffer->size(); i++)
	{
		std::cout << "id : " << (*buffer)[i].id << "\n";

		std::cout << "x : " << (*buffer)[i].x << "\n";
		std::cout << "y : " << (*buffer)[i].y << "\n";
		std::cout << "z : " << (*buffer)[i].z << "\n";

		std::cout << "normal x : " << (*buffer)[i].normal_x << "\n";
		std::cout << "normal y : " << (*buffer)[i].normal_x << "\n";
		std::cout << "normal z : " << (*buffer)[i].normal_x << "\n";

		std::cout << "texture x : " << (*buffer)[i].texture_x << "\n";
		std::cout << "texture y : " << (*buffer)[i].texture_y << "\n\n";
	}
}
