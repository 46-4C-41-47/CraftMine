#include "../include/TextureHandler.h"


TextureHandler* TextureHandler::instance = nullptr;


const std::string TextureHandler::path = "./res/textures/atlas.png";


TextureHandler* TextureHandler::getInstance()
{
    if (instance == nullptr)
        instance = new TextureHandler();

    return instance;
}


Texture* TextureHandler::loadTexture(std::string path) {
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
