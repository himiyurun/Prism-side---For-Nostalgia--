#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <memory>

#include "../GDK/Shader.h"
#include "../Renderer/Texture.h"
#include "../SEL/Structure.h"
#include "Aster.h"

enum enemyDrection
{
    ENEMY_UP,
    ENEMY_DOWN,
    ENEMY_LEFT,
    ENEMY_RIGHT,
    ENEMY_STOP
};

union positionif{
    int positioni[2];
    float positionf[2];
};

struct searchPos
{
    searchPos(float x, float y, Direction dir)
        : position{ x, y }
        , dir(dir)
    {
    }

    float position[2];
    Direction dir;
};

// 敵のクラス
class Enemy
{
    Shader shader;

    GLsizei vertexcount;
    const Object::Vertex *vertex;
    float position[2];

    float positiontip[2][2];

    ShapeIndex shape;
    Texture texture;

    int counter;

    bool exist_stat;

public:

    std::vector<searchPos> search;

    //  コンストラクタ
    //  vsrc : バーテックスシェーダソースファイル名
    //  fsrc : フラグメントシェーダソースファイル名
    //  vertexcount : 頂点の数
    //  vertex : 頂点の座標
    //  indexcount : インデックスの数
    //  index : インデックスの配列
    //  location : オブジェクトの位置
    //  positionld : オブジェクトの左下の位置
    //  positionru : オブジェクトの右上の位置
    //  uv : uv座標
    //  imgname : テクスチャのファイル名
    //  SetUp : シェーダーに値を渡す関数
    Enemy(const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float location[2], float positionld[2], float positionru[2], const Object::Vertex *uv, const char* imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    //  デストラクタ
    ~Enemy();

    //  描画関数
    void active(int *handle, const float size[2], float scale, const float location[2], int texIndex, float Light, int Loc[5]) const
    {
        if(exist_stat)
        {
            shader.use();

            shader.setup(size, scale, position, texture.getTex(), Light, Loc);

            glActiveTexture(texture.getTex());

            texture.bind();

            shape.draw();

            //*handle++;
        }
    }

    bool searchPlayer(std::vector<Points> &point, const int playerLoci[2], const float playerLoc[2], float size[2], float scale);

    bool IntersectPlayer(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const;

    void move(const float size[2], float scale, const float playerLoc[2]);

    void setStat(bool stat) { exist_stat = stat; }

    void setPos(const float pos[2]) { 
        position[0] = pos[0]; 
        position[1] = pos[1]; 
    }

    float getTiltf(const float playerLoc[2]) const 
    {
        if(playerLoc[0] - position[0] > -0.000001f && playerLoc[0] - position[0] < 0.000001f) {
            return (playerLoc[1] - position[1]) / 0.000001f;
        } else if(playerLoc[1] - position[1] < 0.000001f && playerLoc[1] - position[1] > -0.000001f) {
            return 0.000001f;;
        }

        return (playerLoc[1] - position[1]) / (playerLoc[0] - position[0]); 
    }

    void setCounter(int count) { counter = count; }

    int getCount() const { return counter; }

    float getSegmentf(float tlit) const { return position[1] - (tlit * position[0]); }

    GLsizei getVertexCount() const { return vertexcount; }

    const Object::Vertex *getVertex() const { return vertex; }

    const float *getLocPointer() const { return position; }

    bool exist() const { return exist_stat; }
};

#endif // ENEMY_H