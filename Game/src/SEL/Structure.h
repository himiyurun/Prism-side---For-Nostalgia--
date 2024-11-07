#ifndef STRUCTURE_H_
#define STRUCTURE_H_
#include <iostream>
#include <cstdlib>
#include <memory>

#include <glad\glad.h>

#include "../GDK/Shader.h"
#include "../src/Renderer/Index.h"
#include "../src/Renderer/Texture.h"

#define NONE    (0x00)
#define SQUARE  (0x01)
#define QUADS   (0x02)

class Structure
{
    const Object::Vertex *rect;

    Object::Vertex intersect[2];

    size_t vertexcount;

    float location[2];

    const char* imgname;

    const Object::Vertex *uv;

    const size_t shaderindex;

    std::shared_ptr<const ShapeIndex> shape;
    //const ShapeIndex shape;

    std::shared_ptr<const Texture> texture;
    GLuint tbo;

public:

    Structure(size_t vertexcount, const Object::Vertex* rect, float location[2], const char* imgname, const Object::Vertex *uv, GLsizei indexcount, const GLuint *index);

    virtual ~Structure();

    void active(int8_t handle) const
    {
        //texture->activTex(handle);

        //glBindBuffer(GL_ARRAY_BUFFER, tbo);

        shape->draw();
    }

    bool InterSect(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const;

    [[nodiscard]] const Object::Vertex *(getRect() const) { return rect; }

    [[nodiscard]] const Object::Vertex *(getIntersect() const) { return intersect; }

    [[nodiscard]] size_t getCount() const { return vertexcount; }

    [[nodiscard]] const float *(getStrLocation() const) { return location; }

    [[nodiscard]] const char *(getImgName() const) { return imgname; }

    [[nodiscard]] const Object::Vertex *(getUv() const) { return uv; }

    [[nodiscard]] size_t getShaderIndex() const { return shaderindex; }

    [[nodiscard]] Object::Vertex getMin() const { return intersect[0]; }

    [[nodiscard]] Object::Vertex getMax() const { return intersect[1]; }

    //[[nodiscard]] GLuint getTex() const { return texture->getTex(); }

    //[[nodiscard]] int getShaderCount() const { return shaderindex; }
};

#endif  //  _STRUCTURE_H_
