#include "Player.h"

//  constractor
//  vsrc : vertex source filename
//  fsrc : fragment source filename
//  size : Frame Work size
//  vertexcount : points count
//  vertexcount : point attributes
//  indexcount : pointIndex counts
//  index : pointarray Index attributes
//  location : pointer to Player position
//  uv : uv points
//  imgname : texture source filename
Player::Player(const char *vsrc, const char *fsrc, GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float *location, const Object::Vertex *uv, const char *imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
    : shader(new Shader(vsrc, fsrc, SetUp))
    , vertexcount(vertexcount)
    , vertex(vertex)
    , location(location)
    , shape(new ShapeIndex(size, vertexcount, vertex, indexcount, index))
    , texture(new Texture(size, vertexcount, uv, imgname))
{
    loc[0] = shader->getUnfLoc("size");
    loc[1] = shader->getUnfLoc("scale");
    loc[2] = shader->getUnfLoc("location");
    loc[3] = shader->getUnfLoc("tex");
}

void Player::drawCall(float size[2], float scale) const
{
    shader->use();

    //texture->activTex(texture->getTex());
    
    shader->setup(size, scale, location, 0, -1.0f, loc);

    glActiveTexture(GL_TEXTURE0);

    texture->bind();

    shape->draw();
}
