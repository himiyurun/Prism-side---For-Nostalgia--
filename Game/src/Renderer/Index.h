#ifndef  INDEX_H
#define INDEX_H

#include "Shape.h"

class ShapeIndex : public Shape 
{
    GLuint ibo;

protected:

    const GLsizei indexcount;

public:

    ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = nullptr)
        : Shape(size, vertexcount, vertex)
        , indexcount(indexcount)
    {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
    }

    virtual void excute() const
    {
        glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
    }
};

#endif