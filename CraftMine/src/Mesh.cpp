#include "../include/Mesh.h"


Mesh::Mesh(vector<Vertex>* vertices, vector<unsigned int>* textures) 
{
    this->vertices = vertices;
    //this->indices = indices;
    this->textures = textures;

    initMesh();
}


Mesh::Mesh(float* rawData, vector<unsigned int>* textures) {
    vertices = new vector<Vertex>();

    for (int i = 0; i < sizeof(*rawData) / sizeof(float); i + 8) {
        Vertex v = { 
            glm::vec3(rawData[i * 8], rawData[i * 8 + 1], rawData[i * 8 + 2]),
            glm::vec3(rawData[i * 8 + 3], rawData[i * 8 + 4], rawData[i * 8 + 5]),
            glm::vec2(rawData[i * 8 + 6], rawData[i * 8 + 7])
        };

        vertices->push_back(v);
    }

    initMesh();
}


void Mesh::initMesh() 
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*   /!\ A remplacer par GL_DYNAMIC_DRAW /!\   */
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(*vertices)[0], GL_STATIC_DRAW); 

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}


void Mesh::Draw(Shader& shader)
{
    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures->size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, (*textures)[i]);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices->size());
    glBindVertexArray(0);
}
