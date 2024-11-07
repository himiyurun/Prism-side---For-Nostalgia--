#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "../GDK/Shader.h"
#include "../Renderer/Index.h"
#include "../Renderer/Texture.h"

enum playerDrection
{
    PLAYER_UP,
    PLAYER_DOWN,
    PLAYER_LEFT,
    PLAYER_RIGHT,
    PLAYER_STOP
};

class Player {

    std::unique_ptr<const Shader> shader;

    const GLsizei vertexcount;

    const Object::Vertex *vertex;

    const float *location;

    std::unique_ptr<const ShapeIndex> shape;

    std::unique_ptr<const Texture> texture;

    int loc[5];

public:

    Player(const char* vsrc, const char* fsrc, GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float *location, const Object::Vertex *uv, const char* imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    void drawCall(float size[2], float scale) const;

    const float (*getLocPointer() const) { return location; }

    const Object::Vertex *getVertex() const { return vertex; }
};

#endif  //PLAYER_H