//
//  Texture.hpp
//  opengl_learn
//
//  Created by chaojie wu on 11/7/20.
//

#ifndef Texture_h
#define Texture_h

#include <GL/glew.h>
#include <string>
using std::string;

class Texture2D
{
public:
    Texture2D();
    virtual ~Texture2D();
    
    bool loadTexture(const string& filename, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);
    
private:
    
    // Member variable
    GLuint mTexture;
};

#endif /* Texture_h */
