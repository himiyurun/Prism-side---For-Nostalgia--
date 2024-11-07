#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <iostream>
#include <glad/glad.h>



class Object
{
    GLuint vao;

    GLuint vbo;

public:

    struct Vertex
    {
        GLfloat position[2];
    };

    Object(GLint size, GLsizei vertexcount, const Vertex *vertex);

    virtual ~Object();

private:

    Object(const Object &o);
    Object &operator = (const Object &o);

public:

    virtual void bind() const
    {
        glBindVertexArray(vao);
    }
};

#endif //   _OBJECT_H_