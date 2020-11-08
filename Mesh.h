//
//  Mesh.hpp
//  opengl_learn
//
//  Created by chaojie wu on 11/8/20.
//

#ifndef Mesh_h
#define Mesh_h

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    bool loadOBJ(const std::string& filename);
    void draw();

private:
    
    void initBuffers();
    
    // Member variable
    bool mLoaded;
    std::vector<Vertex> mVertices;
    GLuint mVBO, mVAO;
};

#endif /* Mesh_h */

























