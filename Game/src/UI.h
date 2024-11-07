#ifndef UI_H
#define UI_H

#include "GDK/Shader.h"
#include "Renderer/Index.h"
#include "Renderer/Texture.h"

class UI
{
    const Object::Vertex *vertex;

    const GLuint *index;

    const Object::Vertex *uv;

    const GLfloat position[2];

    std::shared_ptr<const Shader> program;

    std::shared_ptr<const ShapeIndex> shape;

    std::shared_ptr<const Texture> texture;

    const void (*UIfunc)(int*, float [2], float);

public:

    UI(const char* vsrc, const char* fsrc, GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locx, float locy, const char* imgname, const Object::Vertex *uv, const void (*func)(int*, float[2], float), const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    virtual void drawUI(float size[2], float scale, int loc[5]) const;

    virtual void excuteUI(int *handle, float MosPos[2], float size[2], float scale) const;
    
    virtual GLboolean getMosIntersect(float MosPos[2], float size[2], float scale) const;

    const float *(getPos() const) { return position; }
};

#endif // UI_H