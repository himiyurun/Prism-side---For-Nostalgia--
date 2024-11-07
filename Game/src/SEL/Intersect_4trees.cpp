//
// Created by rayrk on 24/07/21.
//

#include "Intersect_4trees.h"

Intersect_4trees::Intersect_4trees()
    : location{ 0.0f, 0.0f }
    , size{ 0.0f, 0.0f }
    , min_size{ 0.0f, 0.0f }
    , cell_size(0)
    , level(0)
    , max_cell_size(0)
{
    count_cash[0] = 1;
    for(int i = 1 ; i < MAX_LEVELS + 1; i++)
    {
        count_cash[i] = count_cash[i - 1] * 4;
    }
}

Intersect_4trees::~Intersect_4trees()
{
}

bool Intersect_4trees::Init(int levels, float locationx, float lcoationy, float sizex, float sizey)
{
    if(levels > MAX_LEVELS) return false;

    level = levels;
    location[0] = locationx;
    location[1] = lcoationy;
    size[0] = sizex;
    size[1] = sizey;
    max_cell_size = (count_cash[level] - 0x01) / 0x03;
    min_size[0] = size[0] / (1 << levels);
    min_size[1] = size[1] / (1 << levels);
    cell_size = 1;
    cell.resize(max_cell_size, nullptr);

    return true;
}

bool Intersect_4trees::Regist(ObjList<Structure> *obj)
{
    if(!obj) return false;

    Structure *buf = obj->get();
    if(!buf) return false;

    Object::Vertex min = buf->getMin();
    Object::Vertex max = buf->getMax();

    GLuint mrtn(getMortonNum(min.position[0], min.position[1], max.position[0], max.position[1]));

    if(mrtn == -1) return false;

    if(!cell[mrtn])
    {
        if(!createCell(mrtn)) return false;
    }

    cell[mrtn]->push(obj);
    return true;
}

bool Intersect_4trees::createCell(GLuint morton)
{
    while(!cell[morton])
    {
        cell[morton] = new ColCell<Structure>();

        morton  = (morton - 1) >> 2;
        if(morton >= max_cell_size) return false;
    }
    return true;
}

bool Intersect_4trees::Intersect4TreeS(const float size[2], float scale, const Object::Vertex *rect, const float position[2], float lx, float ly, float rx, float ry) const
{
    GLuint player_morton(getMortonNum(lx, ly, rx, ry));
    
    if(player_morton == -1) return false;
    
    return IntersectStr(player_morton, size, scale, rect, position);
}

bool Intersect_4trees::IntersectStr(GLuint morton, const float size[2], float scale, const Object::Vertex *vertex, const float position[2]) const
{
    GLuint next_morton(0);
    for(int i = 0 ; i < 4; i++)
    {
        if(cell[morton + i])
        {
            ObjList<Structure> *oftf = cell[morton + i]->get();
            if(oftf)
            {
                Structure *buf = nullptr;

                while(oftf)
                {
                    buf = oftf->get();
                    if(buf->InterSect(position, vertex, const_cast<float*>(size), scale)) return true;

                    oftf = oftf->next;
                }
            }
        }

        next_morton = morton * 4 + i + 1;
        if(cell[next_morton])
        {
            if(IntersectStr(next_morton, const_cast<float*>(size), scale, vertex, const_cast<float*>(position))) return true;
        }
    }

    return false;
}

GLuint Intersect_4trees::getMortonNum(float x1, float y1, float x2, float y2) const
{
    x1/=min_size[0];
    y1/=min_size[1];
    x2/=min_size[0];
    y2/=min_size[1];

    GLuint sellIndex1(getSlideMorton(static_cast<GLuint>(x1), static_cast<GLuint>(y1)));
    GLuint sellIndex2(getSlideMorton(static_cast<GLuint>(x2), static_cast<GLuint>(y2)));

    if(sellIndex1 >= max_cell_size || sellIndex2 >= max_cell_size) return -1;

    GLuint morton(sellIndex1 ^ sellIndex2);
    unsigned short shift_level(0);
    for(int i = 0; i < level; i++)
    {
        if(((morton >> i * 2) & 0x03))
        {
            shift_level = i + 1;
            break;
        }
    }

    GLuint space_location(sellIndex1 >> (2 * shift_level));
    space_location += ((count_cash[level - shift_level] - 0x01) / 0x03);

    if(space_location >= max_cell_size) return -1;

    return space_location;
}

GLuint Intersect_4trees::getSlideMorton(GLuint x, GLuint y) const
{
    return getSeparateBits(x) | (getSeparateBits(y) << 1);
}

uint32_t Intersect_4trees::getSeparateBits(GLuint morton) const
{
    morton = (morton | (morton << 8)) & 0x00ff00ff;
    morton = (morton | (morton << 4)) & 0x0f0f0f0f;
    morton = (morton | (morton << 2)) & 0x33333333;
    return (morton | (morton << 1)) & 0x55555555;
}
