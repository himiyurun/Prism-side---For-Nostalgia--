//
// Created by rayrk on 24/09/01.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <glad\glad.h>

#include "Object.h"

class Texture
{
    GLuint tbo;

    GLuint tex;

    int bufSize[2];

    std::vector<GLubyte> img;

public:
    Texture(GLint size, const GLsizei uvcount, const Object::Vertex *uv, const char *imgname);

    bool readBMP(const char* imgname, std::vector<GLubyte> &img, int size[2]) const;

    virtual void bind() const
    {
        //glBindBuffer(GL_ARRAY_BUFFER, tbo);

        glBindTexture(GL_TEXTURE_2D, tex);
    }

    void activTex(int8_t handle) const
    {
        glActiveTexture(handle);
        
        bind();

        glUniform1i(3, handle);
    }

    GLuint getTex() const { return tex; }
};
#endif //TEXTURE_H
