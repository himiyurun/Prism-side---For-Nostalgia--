//
// Created by rayrk on 24/07/21.
//

#ifndef INTERSECT_H
#define INTERSECT_H

#define MAX_LEVELS 9

#include <vector>

#include "objlist.h"
#include "Structure.h"

class Intersect_4trees
{
    //  空間の数
    std::vector<ColCell<Structure>*> cell;
    //  4の等比数列の和のキャッシュ
    uint32_t count_cash[MAX_LEVELS + 1];
    //  ローカル空間の左上の座標
    float location[2];
    //  ローカル空間のサイズ
    float size[2];
    //  最小の空間のサイズ
    float min_size[2];
    //  セルのサイズ
    uint8_t cell_size;
    //  空間のレベル
    int level;
    //  空間の最大数
    int max_cell_size;

public:
    //  コンストラクタ
    //  値を初期化する
    Intersect_4trees();

    //  デストラクタ
    ~Intersect_4trees();

    //  初期化
    //  levels : 空間のレベル
    //  locationx : ローカル空間の左上のx座標
    //  locationy : ローカル空間の左上のy座標
    //  sizex : ローカル空間のx方向のサイズ
    //  sizey : ローカル空間のy方向のサイズ
    bool Init(int levels, float locationx, float lcoationy, float sizex, float sizey);

    //  オブジェクトの割り当て
    //  obj : 割り当てるオブジェクト
    bool Regist(ObjList<Structure> *obj);

    //  新しい空間の作成
    //  morton : モートン番号
    bool createCell(GLuint morton);

    //  当たり判定を取る
    //  size : ウインドウのサイズ
    //  scale : ウインドウのスケール
    //  position : プレイヤーの位置
    //  lx : プレイヤーの左端のx座標
    //  ly : プレイヤーの左端のy座標
    //  rx : プレイヤーの右端のx座標
    //  ry : プレイヤーの右端のy座標
    bool Intersect4TreeS(const float size[2], float scale, const Object::Vertex rect[2], const float position[2], float lx, float ly, float rx, float ry) const;

    //  ストラクチャーの衝突判定関数を実行
    //  obj : 衝突判定を取るストラクチャー
    bool IntersectStr(GLuint morton, const float size[2], float scale, const Object::Vertex *vertex, const float position[2]) const;

    //  モートン番号を取得
    //  x : x座標
    //  y : y座標
    GLuint getMortonNum(float x1, float y1, float x2, float y2) const;

    //  スライドしてモートン番号に変換
    //  morton : モートン番号
    GLuint getSlideMorton(GLuint x, GLuint y) const;

    uint32_t getSeparateBits(GLuint morton) const;
}; 

#endif // INTERSECT_H
