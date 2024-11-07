#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "../SEL/Structure.h"

#define MAP_HEIGHT  (15)
#define MAP_WIDTH   (15)

#define MAP_ROOM_COUNT  (5)
#define MAP_ROOM_SIZE  (5)

#define CLEAR_TILE      (16)
#define CLEAR_TILE_STAR (6)

enum MAP_TYPE
{
    MAP_WALL,
    MAP_ROAD,
    MAP_OBJECT_TABLE,
    MAP_OBJECT_RAMP
};

struct MapRoomInfo {
    int size[2];
    int layout[MAP_ROOM_SIZE * MAP_ROOM_SIZE];
};

struct MapRect {
    MapRect(const Object::Vertex vertex[4], const Object::Vertex uv[4])
        : vertex{ vertex[0], vertex[1], vertex[2], vertex[3] }
        , uv{ uv[0], uv[1], uv[2], uv[3] }
    {
    }

    const Object::Vertex vertex[4];
    const Object::Vertex uv[4];
};

struct ClearTile{
    int x;
    int y;
    uint8_t is_used;
};

// マップの迷路を生成するクラス
class Map
{
    static const MapRoomInfo room[MAP_ROOM_SIZE];

    const float size[2];

    uint8_t tileStat[MAP_HEIGHT][MAP_WIDTH];

    ClearTile clearTile[CLEAR_TILE];

    int8_t clearcount;

    ClearTile cleartileStar[CLEAR_TILE_STAR];

    int8_t clearcountStar;

public:

    int field[MAP_HEIGHT][MAP_WIDTH];

    Map(float map_Height, float map_Width);

    ~Map();

    void setStart(float position[2], int x, int y, float size[2], float scale) const;

    void createMap();

    bool collision(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const;

    int calcTilex(float position, float location, float beg, float div, const float size[2], float scale) const { return static_cast<int>((position * scale / size[0] + location + beg * scale / size[0]) / (div * scale / size[0])); }

    int calcTiley(float position, float location, float beg, float div, const float size[2], float scale) const { return (MAP_HEIGHT - 1) - static_cast<int>((position * scale / size[1] + location + beg * scale / size[1]) / (div * scale / size[1])); }
    
    void rigistStar(int x, int y);

    void rigistClear(int x, int y);

    virtual void makeMaze();

    virtual void makeRoom(int roomcount);

    virtual void dig(int x, int y);

    virtual bool boolRoom(int x, int y, int index) const;

    virtual bool collisionPosition(int x, int y) const;

    virtual void writeRoom(int x, int y, int index);

    virtual void drawField() const;

    void setStat(uint8_t stat, int x, int y) { tileStat[y][x] = stat; }

    void getRoomPos(int pos[2]) const;
    
    int getLandm(int offset, int div) const { return rand() % div + offset; }

    int getTipPoint(int num, int max) const { return (max - 2) - num; }

    uint8_t getStat(int x, int y) const { return tileStat[y][x]; }
};

#endif // MAP_H