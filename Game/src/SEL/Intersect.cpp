#include "Intersect.h"

/*
template <typename T>
IntersectTile<T>::IntersectTile(float location[2], float size[2])
    : location(location)
    , size(size)
    , tile_size{ size[0] / DIV_WIDTH, size[1] / DIV_HEIGHT }
    , array(DIV_HEIGHT * DIV_WIDTH)
{

}
*/

/*
template <typename T>
IntersectTile<T>::~IntersectTile()
{
}
*/

/*
template <typename T>
void IntersectTile<T>::Rigist(T* obj, uint32_t index, float lx, float ly, float rx, float ry)
{   
    alignmentPos(lx, ly, rx, ry);

    uint32_t tile_indexl[2]{ static_cast<uint32_t>((lx + 1.f) / tile_size[0]), DIV_HEIGHT - static_cast<uint32_t>((ly + 1.f) / tile_size[1]) };
    uint32_t tile_indexr[2]{ static_cast<uint32_t>((rx + 1.f) / tile_size[0]), DIV_HEIGHT - static_cast<uint32_t>((ry + 1.f) / tile_size[1]) };

    uint32_t width = tile_indexr[0] - tile_indexl[0];
    uint32_t height = tile_indexl[1] - tile_indexr[1];

    for(int i = 0 ; i < height ; i++) {
        for(int j = 0 ; j < width ; j++) {
            array[tile_indexr[1] + i][tile_indexl[0] + j].emplace_back(index);
        }
    }

    std::cout << "x : " << tile_indexl[0] << ", y : " << tile_indexr[1] << ", width : " << width << ", height : " << height << std::endl;

    str_array.emplace_back(obj, tile_indexl[0], tile_indexr[1], width, height);
}
*/

/*
template <typename T>
void IntersectTile<T>::alignmentPos(float lx, float ly, float rx, float ry) const
{
        if(lx < -1.f)
    {
        lx = -1.f;
    } else if(lx > 1.f) 
    {
        lx = 1.f;
    }

    if(ly < -1.f)
    {
        ly = -1.f;
    } else if(ly > 1.f) 
    {
        ly = 1.f;
    }

    if(rx < -1.f)
    {
        rx = -1.f;
    } else if(rx > 1.f) 
    {
        rx = 1.f;
    }

    if(ry < -1.f)
    {
        ry = -1.f;
    } else if(ry > 1.f) 
    {
        ry = 1.f;
    }
}
*/

/*
template <typename T>
void IntersectTile<T>::locateTile(std::vector<uint32_t> *num, float lx, float ly, float rx, float ry) const
{   
    alignmentPos(lx, ly, rx, ry);
    std::cout<<"lx : "<<lx<<", ly : "<<ly<<", rx : "<<rx<<", ry : "<<ry<<std::endl;
    system("cls");

    uint32_t tile_indexl[2] = { static_cast<uint32_t>(abs((lx + 1.f) / tile_size[0])), static_cast<uint32_t>(abs((ly + 1.f) / tile_size[1])) };
    uint32_t tile_indexr[2] = { static_cast<uint32_t>(abs((rx + 1.f) / tile_size[0])), static_cast<uint32_t>(abs((ry + 1.f) / tile_size[1])) };

    std::cout<<"tile_indexl : "<<tile_indexl[0]<<", "<<tile_indexl[1]<<std::endl;
    std::cout<<"tile_indexr : "<<tile_indexr[0]<<", "<<tile_indexr[1]<<std::endl;

    //  配列は0スタート、よって1を足す
    uint32_t width = abs(static_cast<int>(tile_indexr[0]) - static_cast<int>(tile_indexl[0])) + 1;
    uint32_t height = abs(static_cast<int>(tile_indexl[1]) - static_cast<int>(tile_indexr[1])) + 1;

    std::cout<<"width : "<<width<<", height : "<<height<<std::endl;

    for(int i = 0 ; i < height ; i++) {
        for(int j = 0; j < width; j++) {
            std::cout << "x : " << tile_indexl[0] + j << ", y : " << tile_indexr[1] + i << std::endl;
            for(auto &a : array[tile_indexr[1]][tile_indexl[0]]) {
                num->emplace_back(a);
            }
        }
    }
}
*/