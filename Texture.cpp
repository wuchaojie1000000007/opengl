//
//  Texture.cpp
//  opengl_learn
//
//  Created by chaojie wu on 11/7/20.
//

// Define this or stb_image will not work
#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include <iostream>
#include <GL/stb_image.h>


Texture2D::Texture2D()
    : mTexture(0)
{
    
}

Texture2D::~Texture2D()
{
    
}

// Find some library to deal with image.
bool Texture2D::loadTexture(const string& filename, bool generateMipMaps)
{
    int width, height, components;
    
    unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
    if (imageData == NULL) {
        std::cerr << "Error loading texture, filename: " << filename << "\n";
        return false;
    }
    
    // Have get image data
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    
    // Set param of this texture instance
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Image should flip upside down or map it upside down
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    // Free Memory
    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}

// Deal with multiple texture, bind them to different place
void Texture2D::bind(GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2D::unbind(GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
