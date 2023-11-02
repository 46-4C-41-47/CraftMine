#include "../include/TextureLoader.h"


TextureLoader* TextureLoader::instance = nullptr;


const std::vector<std::string> TextureLoader::paths = {
    "./res/textures/atlas.png",
    /*
    "./res/textures/atlas.png",
    "./res/textures/dirt.jpg",
    "./res/textures/grass.jpg",
    "./res/textures/stone.jpg",
    "./res/textures/water.jpg",
    "./res/textures/lava.jpg",
    "./res/textures/wood.jpg",
    "./res/textures/leaf.jpg",
    "./res/textures/diamond.jpg"
    */
};


TextureLoader::TextureLoader()
{
    for (int i = 0; i < paths.size(); i++)
        textures.push_back(loadTexture(paths[i]));
}


TextureLoader::~TextureLoader()
{
    for (int i = 0; i < textures.size(); i++)
        delete textures[i];
}


TextureLoader* TextureLoader::getInstance()
{
    if (instance == nullptr)
        instance = new TextureLoader();

    return instance;
}


Texture* TextureLoader::loadTexture(std::string path) {
    unsigned int address;
    int width, height, nrChannels;
    unsigned char* data;

    glGenTextures(1, &address);
    glBindTexture(GL_TEXTURE_2D, address);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture, path : " << path << "\n";
        exit(1);
    }
    stbi_image_free(data);

    return new Texture{ address, width, height };
}
