//
//  ShaderProgram.hpp
//  opengl_learn
//
//  Created by chaojie wu on 11/6/20.
//

#ifndef ShaderProgram_h
#define ShaderProgram_h

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <map>
using std::string;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    
    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };
    
    bool loadShaders(const char* vsFilename, const char* fsFilename);
    void use();
    
    // use glm vec type to get glsm vec type, pass it to shader uniform.
    void setUniform(const GLchar* name, const glm::vec2& v);
    void setUniform(const GLchar* name, const glm::vec3& v);
    void setUniform(const GLchar* name, const glm::vec4& v);
    void setUniform(const GLchar* name, const glm::mat4& v);
    
    GLuint getProgram()const;
    
private:
    
    string fileToString(const string& filename);
    void checkCompileErrors(GLuint shader, ShaderType type);
    GLuint getUniformLocation(const GLchar* name);
    
    // Member variable
    GLuint mHandle;
    std::map<string, GLuint> mUniformLocations;
    
};


#endif /* ShaderProgram_h */
