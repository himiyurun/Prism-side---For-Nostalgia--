#ifndef SHAPE_H
#define SHAPE_H
#include <memory>
#include "Object.h"

class Shape
{
    std::shared_ptr<const Object> object;

protected:

    GLsizei vertexcount;

public:

    Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
        : object(new Object(size, vertexcount, vertex))
        , vertexcount(vertexcount)
    {

    }

    virtual void excute() const
    {
        glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
    }

    virtual void bind() const
    {
        object->bind();
    }
    
    void draw() const
    {
        object->bind();

        excute();
    }
};

#endif