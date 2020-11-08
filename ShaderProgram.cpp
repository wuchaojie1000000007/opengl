//
//  ShaderProgram.cpp
//  opengl_learn
//
//  Created by chaojie wu on 11/6/20.
//

#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

// Init()
ShaderProgram::ShaderProgram()
    : mHandle(0)
{
    
}

// Delete()
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mHandle);
}

// File -> String -> Char Pointer -> Shader -> Program.
bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
    string vsString = fileToString(vsFilename);
    string fsString = fileToString(fsFilename);
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();
    
    // Create shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Put source into shader
    glShaderSource(vs, 1, &vsSourcePtr, NULL);
    glShaderSource(fs, 1, &fsSourcePtr, NULL);
    
    // Compile Shader and Check Shader state
    glCompileShader(vs);
    checkCompileErrors(vs, VERTEX);
    glCompileShader(fs);
    checkCompileErrors(fs, FRAGMENT);
    
    
    // create and link program
    mHandle = glCreateProgram();
    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);
    glLinkProgram(mHandle);
    
    // Check program state.
    checkCompileErrors(mHandle, PROGRAM);
    
    // Do not need shader any more, just use it to create program.
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    mUniformLocations.clear();
    
    return true;
}

void ShaderProgram::use()
{
    if (mHandle > 0) {
        glUseProgram(mHandle);
    }
}

// I have shader src file, want to make it to string, then use string to create shader.
string ShaderProgram::fileToString(const string& filename)
{
    std::stringstream ss;
    std::ifstream file;
    
    try {
        file.open(filename, std::ios::in);
        
        if (!file.fail()) {
            ss << file.rdbuf();
        }
        
        file.close();
    } catch (std::exception ex) {
        std::cerr << "Error reading shader filename!" << "\n";
    }
    
    return ss.str();
}

// Check if shader or program is ok to use.
void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
    int status = 0;
    
    if (type == PROGRAM)
    {
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            // Get log length
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
            
            // Get log infomation and print it
            string errorLog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
            std::cerr << "Error! program failed to link." << errorLog << "\n";
        }
    }
    else // check vertes or fragment
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            // Get log length
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            
            // Get log infomation and print it
            string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile." << errorLog << "\n";
        }
    }
}

// Get uniform location through map or create it.
GLuint ShaderProgram::getUniformLocation(const GLchar* name)
{
    std::map<string, GLuint>::iterator it = mUniformLocations.find(name);
    
    // Not in the map, then create it.
    if (it == mUniformLocations.end())
    {
        mUniformLocations[name] = glGetUniformLocation(mHandle, name);
    }
    
    return mUniformLocations[name];
}

GLuint ShaderProgram::getProgram()const
{
    return mHandle;
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
    GLint loc = getUniformLocation(name);
    // Why not just use vector?
    glUniform2f(loc, v.x, v.y);
}
    
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}
