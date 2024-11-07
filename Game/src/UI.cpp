#include "UI.h"

UI::UI(const char *vsrc, const char *fsrc, GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locx, float locy, const char *imgname, const Object::Vertex *uv, const void (*func)(int*, float[2], float), const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
    : vertex(vertex)
    , index(index)
    , uv(uv)
    , position{ locx, locy }
    , program(new Shader(vsrc, fsrc, SetUp))
    , shape(new ShapeIndex(size, vertexcount, vertex, indexcount, index))
    , texture(new Texture(size, vertexcount, uv, imgname))
    , UIfunc(func)
{
}

void UI::drawUI(float size[2], float scale, int loc[5]) const
{
    program->use();

    program->setup(size, scale, position, 16, -1.0f, loc);

    //texture->activTex();

    glActiveTexture(GL_TEXTURE0 + 16);

    texture->bind();

    glUniform1i(3, 16);

    shape->draw();
}

void UI::excuteUI(int * handle, float MosPos[2], float size[2], float scale) const
{
    if(getMosIntersect(MosPos, size, scale))
        UIfunc(handle, size, scale);
}

GLboolean UI::getMosIntersect(float MosPos[2], float size[2], float scale) const
{
    if( (vertex + 1)->position[0] * scale / size[0] + position[0] < MosPos[0] &&
        (vertex + 2)->position[0] * scale / size[0] + position[0] > MosPos[0] &&
        (vertex + 0)->position[1] * scale / size[1] + position[1] > MosPos[1] &&
        (vertex + 1)->position[1] * scale / size[1] + position[1] < MosPos[1])
        return true;
    return false;
}
