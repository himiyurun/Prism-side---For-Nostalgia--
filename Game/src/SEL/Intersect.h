#ifndef INTERSECT_H
#define INTERSECT_H

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>

#define DIV_HEIGHT  (4)
#define DIV_WIDTH   (8)

template <typename T>
struct StrInfo
{
    StrInfo(T* obj, uint32_t xindex, uint32_t yindex, uint32_t size_x, uint32_t size_y)
        : obj(obj)
        , beg{ xindex, yindex }
        , size{ size_x, size_y }
        , insec(false)
    {}

    T* obj;
    uint32_t beg[2];
    uint32_t size[2];

    bool insec;
};

template<typename T>
class IntersectTile
{
    float location[2];

    float size[2];

    float tile_size[2];

public:

    std::vector<uint32_t> array[DIV_HEIGHT][DIV_WIDTH];

    std::vector<StrInfo<T>> str_array;

    //  コンストラクタ
    //  location : 左上の座標
    //  size : タイル全体のサイズ
    IntersectTile(float location[2], float size[2])
        : location{ location[0], location[1] }
        , size{ size[0], size[1] }
        , tile_size{ size[0] / DIV_WIDTH, size[1] / DIV_HEIGHT }
    {
    }

    //  デストラクタ
    //~IntersectTile();

    //  オブジェクトを入れる
    void Rigist(T* obj, uint32_t index, float lx, float ly, float rx, float ry)
    {   
        alignmentPos(lx, ly, rx, ry);

        uint32_t tile_indexl[2]{ static_cast<uint32_t>((lx + 1.f) / tile_size[0]), (DIV_HEIGHT - 1) - static_cast<uint32_t>((ly + 1.f) / tile_size[1]) };
        uint32_t tile_indexr[2]{ static_cast<uint32_t>((rx + 1.f) / tile_size[0]), (DIV_HEIGHT - 1) - static_cast<uint32_t>((ry + 1.f) / tile_size[1]) };

        uint32_t width = abs(static_cast<int>(tile_indexr[0]) - static_cast<int>(tile_indexl[0])) + 1;
        uint32_t height = abs(static_cast<int>(tile_indexl[1]) - static_cast<int>(tile_indexr[1])) + 1;

        for(int i = 0 ; i < height ; i++) {
            for(int j = 0 ; j < width ; j++) {
                //std::cout << "x : " << tile_indexl[0] + j << ", y : " << tile_indexr[1] + i << std::endl;

                array[tile_indexr[1] + i][tile_indexl[0] + j].emplace_back(index);
            }
        }

        //std::cout << "x : " << tile_indexl[0] << ", y : " << tile_indexr[1] << ", width : " << width << ", height : " << height << std::endl;

        str_array.emplace_back(obj, tile_indexl[0], tile_indexr[1], width, height);

        //std::cout<<"str_array size : "<<str_array.size()<<std::endl;
    }

    //  オブジェクトを外す
    //  index : オブジェクトのインデックス
    //  ※ 線形検索のため遅い、使用は非推奨
    void unRigist(uint32_t index);

    //  オブジェクトをクリアする
    void clear()
    {
        for(auto &a : array) {
            for(auto &b : a) {
                b.clear();
            }
        }
    }

    //  対象のタイルのオブジェクトを取得する
    //  num : オブジェクトのインデックスを入れる
    //  lx, ly : 左下の座標
    //  rx, ry : 右上の座標
    void locateTile(std::vector<uint32_t> &num, float lx, float ly, float rx, float ry) const
    {   
        alignmentPos(lx, ly, rx, ry);
        //std::cout<<"lx : "<<lx<<", ly : "<<ly<<", rx : "<<rx<<", ry : "<<ry<<std::endl;

        uint32_t tile_indexl[2] = { static_cast<uint32_t>(abs((lx + 1.f) / tile_size[0])), (DIV_HEIGHT - 1) - static_cast<uint32_t>(abs((ly + 1.f) / tile_size[1])) };
        uint32_t tile_indexr[2] = { static_cast<uint32_t>(abs((rx + 1.f) / tile_size[0])), (DIV_HEIGHT - 1) - static_cast<uint32_t>(abs((ry + 1.f) / tile_size[1])) };

        //std::cout<<"tile_indexl : "<<tile_indexl[0]<<", "<<tile_indexl[1]<<std::endl;
        //std::cout<<"tile_indexr : "<<tile_indexr[0]<<", "<<tile_indexr[1]<<std::endl;

        //  配列は0スタート、よって1を足す
        uint32_t width = abs(static_cast<int>(tile_indexr[0]) - static_cast<int>(tile_indexl[0])) + 1;
        uint32_t height = abs(static_cast<int>(tile_indexl[1]) - static_cast<int>(tile_indexr[1])) + 1;

        //std::cout<<"width : "<<width<<", height : "<<height<<std::endl;

        for(int i = 0 ; i < height ; i++) {
            for(int j = 0; j < width; j++) {
                //std::cout << "x : " << tile_indexl[0] + j << ", y : " << tile_indexr[1] + i << std::endl;

#ifdef _DEBUG
                if(tile_indexr[1] + i >= DIV_HEIGHT || tile_indexl[0] + j >= DIV_WIDTH) {
                    std::cout<<"out of range"<<std::endl;
                    continue;
                }
#endif

                for(uint32_t a : array[tile_indexr[1] + i][tile_indexl[0] + j]) {
                    num.emplace_back(a);
                }
            }
        }
    }

    void alignmentPos(float &lx, float &ly, float &rx, float &ry) const
    {
        if(lx <= -1.f)
        {
            lx = -0.99999f;
        } else if(lx >= 1.f) 
        {
            lx = 0.99999f;
        }

        if(ly <= -1.f)
        {
            ly = -0.99999f;
        } else if(ly >= 1.f) 
        {
            ly = 0.99999f;
        }

        if(rx <= -1.f)
        {
            rx = -0.99999f;
        } else if(rx >= 1.f) 
        {
            rx = 0.99999f;
        }

        if(ry <= -1.f)
        {
            ry = -0.99999f;
        } else if(ry >= 1.f) 
        {
            ry = 0.99999f;
        }

    }

    //  オブジェクト配列を取得する
    std::vector<T*>* getArray() const { return &array; }
};

#endif //   INTERSECT_H