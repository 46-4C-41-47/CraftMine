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


int ChunkMeshBuffer::getInterpolationIndex(unsigned int id)
{
	for (int i = 0; i < buffer->size(); i++)
	{
		if (id < (*buffer)[i].id)
		{
			return i;
		}
	}
}


int ChunkMeshBuffer::find(unsigned int id)
{
	int start = 0, end = buffer->size() - 1, middle;
	
	while (start <= end)
	{
		middle = floor((start + end) / 2);

		if ((*buffer)[middle].id == id)
			return middle;

		else if (id < (*buffer)[middle].id)
			end = middle - 1;
		
		else
			start = middle + 1;
	}

	return -1;
}


void ChunkMeshBuffer::insertFace(int x, int y, int z, int face_index)
{
	int index = getInterpolationIndex(func::getBufferId(x, y, z, face_index, 0));

	for (int i = 0; i < cube_face_size / 8; i++)
	{
		int vertex_index = face_index * cube_face_size + i * 8;

		BufferVertex bv = {
			func::getBufferId(x, y, z, face_index, i),

			cube_vertices[vertex_index + 0] + (float)x,
			cube_vertices[vertex_index + 1] + (float)y,
			cube_vertices[vertex_index + 2] + (float)z,

			cube_vertices[vertex_index + 3],
			cube_vertices[vertex_index + 4],
			cube_vertices[vertex_index + 5],
			cube_vertices[vertex_index + 6],
			cube_vertices[vertex_index + 7]
		};

		buffer->insert(buffer->begin() + index + i, bv);
	}
}


void ChunkMeshBuffer::removeFace(unsigned int id)
{
	int offset = find(id);

	if (offset != -1)
		buffer->erase(buffer->begin() + offset, buffer->begin() + offset + 6);
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
		std::cout << "normal y : " << (*buffer)[i].normal_y << "\n";
		std::cout << "normal z : " << (*buffer)[i].normal_z << "\n";

		std::cout << "texture x : " << (*buffer)[i].texture_x << "\n";
		std::cout << "texture y : " << (*buffer)[i].texture_y << "\n\n";
	}
}
