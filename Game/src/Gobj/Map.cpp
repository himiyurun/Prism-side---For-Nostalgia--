#include "Map.h"

const MapRoomInfo Map::room[MAP_ROOM_SIZE] = {
        {{3, 3}, {  1, 1, 1,
                    1, 1, 1,
                    1, 1, 1 }},
        {{3, 3}, {  1, 1, 1,
                    1, 1, 1,
                    1, 1, 1 }},
        {{5, 5}, {  1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1, 
                    1, 1, 1, 1, 1, 
                    1, 1, 1, 1, 1, 
                    1, 1, 1, 1, 1 }},
        {{3, 5}, {  1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1 }},
        {{5, 5}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}}
    };

Map::Map(float map_Height = 2.f, float map_Width = 2.f)
    : size{ map_Width, map_Height }
    , clearcount(0)
    , clearcountStar(0)
{
    memset(field, 0, sizeof(field));

    memset(tileStat, 0, sizeof(tileStat));

    for(int i = 0; i < CLEAR_TILE; i++)
    {
        clearTile[i].is_used = false;
    }
    for(int i = 0; i < CLEAR_TILE_STAR; i++)
    {
        cleartileStar[i].is_used = false;
    }
    
    //srand(static_cast<unsigned>(time(nullptr)));
}

Map::~Map()
{
}

void Map::setStart(float position[2], int x, int y, float size[2], float scale) const
{
    float div(15.f / MAP_WIDTH);

    position[0] = ((x * div + (div / 2.f)) - 7.5f) * scale / size[0];
    position[1] = (7.5f - (y * div + (div / 2.f))) * scale / size[1];
}

void Map::createMap()
{
    makeMaze();

    //makeRoom(1);

    drawField();
}

bool Map::collision(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const
{
    float div(15.f / MAP_WIDTH);
    float divLoc(15.f / 2.f);

    int pos[4]{
        calcTilex(playerRect[2].position[0], playerLoc[0], divLoc, div, size, scale), calcTiley(playerRect[2].position[1], playerLoc[1], divLoc, div, size, scale),
        calcTilex(playerRect[0].position[0], playerLoc[0], divLoc, div, size, scale), calcTiley(playerRect[0].position[1], playerLoc[1], divLoc, div, size, scale)
    };
    
    if(collisionPosition(pos[0], pos[1]) || collisionPosition(pos[2], pos[1]) || collisionPosition(pos[2], pos[3]) || collisionPosition(pos[0], pos[3]))
    {
        return true;
    }

    return false;
}

void Map::rigistStar(int x, int y)
{
    if(clearcountStar >= CLEAR_TILE_STAR)
    {
        clearcountStar = 0;
    }
    int count = clearcount;
    
    if(cleartileStar[clearcountStar].is_used == true)
    {
        //std::cout<<"init"<<std::endl;
        //std::cout<<"x : "<<cleartileStar[clearcountStar].x<<", y : "<<cleartileStar[clearcountStar].y<<std::endl;
        setStat(0, cleartileStar[clearcountStar].x, cleartileStar[clearcountStar].y);
    }

    //if(count == 0) count = CLEAR_TILE;

    if(!getStat(clearTile[count - 1].x, clearTile[count - 1].y))
    {
        std::cout<<"error"<<std::endl;
        std::cout<<"x : "<<clearTile[count - 1].x<<", y : "<<clearTile[count - 1].y<<std::endl;
        return;
    }

    if(getStat(clearTile[count - 1].x, clearTile[count - 1].y) != 3)
    {
        cleartileStar[clearcountStar].x = clearTile[count - 1].x;
        cleartileStar[clearcountStar].y = clearTile[count - 1].y;
        cleartileStar[clearcountStar].is_used = true;
        setStat(3, cleartileStar[clearcountStar].x, cleartileStar[clearcountStar].y);

        clearTile[count - 1].is_used = false;
        clearcountStar++;
    }
}

void Map::rigistClear(int x, int y)
{
    if(clearcount >= CLEAR_TILE)
    {
        clearcount = 0;
    }

    if(getStat(x, y))
    {
        return;
    }

    if(clearTile[clearcount].is_used)
    {
        setStat(0, clearTile[clearcount].x, clearTile[clearcount].y);
    }
        
    clearTile[clearcount].x = x;
    clearTile[clearcount].y = y;
    clearTile[clearcount].is_used = true;
    setStat(1, x, y);

    clearcount++;
}

void Map::makeMaze()
{
    int beg[2]{ getLandm(0, MAP_WIDTH / 2 - 1) * 2 + 1, getLandm(0, MAP_HEIGHT / 2 - 1) * 2 + 1 };

    field[beg[1]][beg[0]] = 1;
    dig(beg[0], beg[1]);
}

void Map::makeRoom(int roomcount)
{
    static const int minSize = 3;
    static const int maxSize = 5;

    int roompos[MAP_ROOM_COUNT][2];

    if(roomcount > MAP_ROOM_COUNT)
    {
        roomcount = MAP_ROOM_COUNT;
    }

    int counter = 0;

    int pos[2]{ 0, 0 };
    getRoomPos(pos);

    bool flag = boolRoom(pos[0], pos[1], 0);
    writeRoom(pos[0], pos[1], 0);

    pos[0] = MAP_WIDTH - getLandm(0, MAP_WIDTH / 4 - 1) * 2 + 1;
    pos[1] = MAP_HEIGHT - getLandm(0, MAP_HEIGHT / 4 - 1) * 2 + 1;

    for(int i = 1; i < roomcount; i++)
    {
        flag = false;

        while(!flag) {
            if(counter > 100)
            {
                break;
            }
            
            flag = boolRoom(pos[0], pos[1], i);

            if(flag)
            {
                writeRoom(pos[0], pos[1], i);
                break;
            }

            getRoomPos(pos);
            counter++;
        }
        counter = 0;
    }
}

void Map::dig(int x, int y)
{
    int direc;
	bool count[4]{ false, false, false, false };

	//std::cout << "x : " << x << ", y : " << y << std::endl;

	while (count[0] == false || count[1] == false || count[2] == false || count[3] == false) {
		direc = getLandm(0, 4);

		switch (direc) {
		case 0:
			if (y - 2 >= 0 && y - 2 < MAP_HEIGHT && field[y - 2][x] == 0) {
				field[y - 1][x] = 1;
				field[y - 2][x] = 1;
				//drawScreen();
				dig(x, y - 2);
			}
			count[0] = true;
			break;
		case 1:
			if (x + 2 >= 0 && x + 2 < MAP_WIDTH && field[y][x + 2] == 0) {
				field[y][x + 1] = 1;
				field[y][x + 2] = 1;
				//drawScreen();
				dig(x + 2, y);
			}
			count[1] = true;
			break;
		case 2:
			if (y + 2 >= 0 && y + 2 < MAP_HEIGHT && field[y + 2][x] == 0) {
				field[y + 1][x] = 1;
				field[y + 2][x] = 1;
				//drawScreen();
				dig(x, y + 2);
			}
			count[2] = true;
			break;
		case 3:
			if (x - 2 >= 0 && x - 2 < MAP_WIDTH && field[y][x - 2] == 0) {
				field[y][x - 1] = 1;
				field[y][x - 2] = 1;
				//drawScreen();
				dig(x - 2, y);
			}
			count[3] = true;
			break;
		}
	}
}

bool Map::boolRoom(int x, int y, int index) const
{
    bool flag = false;

    if( x > 0 && x < MAP_WIDTH - 1 && x + room[index].size[0] > 0 && x + room[index].size[0] < MAP_WIDTH - 1 && 
        y > 0 && y < MAP_HEIGHT - 1 && y + room[index].size[1] > 0 && y + room[index].size[1] < MAP_HEIGHT - 1)
    {
        flag = true;
        for(int j = 0; j < room[index].size[1]; j++) {
            for(int k = 0; k < room[index].size[0]; k++)
            {
                if(field[y + j][x + k] != 0 && field[y + j][x + k] == 2)
                {
                    flag = false;
                    break;
                }
            }
        }
    }

    return flag;
}

bool Map::collisionPosition(int x, int y) const
{
    if(x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
    {
        if(field[y][x] == MAP_WALL)
        {
            return true;
        }
    }
    return false;
}

void Map::writeRoom(int x, int y, int index)
{
    for(int i = 0; i < room[index].size[1]; i++) {
        for(int j = 0; j < room[index].size[0]; j++)
        {
            //field[y + i][x + j] = room[index].layout[i * room[index].size[0] + j];
            field[y + i][x + j] = 2;
        }
    }
}

void Map::drawField() const
{
    for(int i = 0; i < MAP_HEIGHT; i++) {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            switch(field[i][j])
            {
            case 0:
                std::cout << "#";
                break;
            case 1:
                std::cout << " ";
                break;
            case 2:
                std::cout<<"^";
                break;
            default:
                std::cout << "?";
                break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Map::getRoomPos(int pos[2]) const
{
    switch(getLandm(0, 4))
    {
    case 0:
        pos[0] = getLandm(0, MAP_WIDTH / 4 - 1) * 2 + 1;
        pos[1] = getLandm(0, MAP_HEIGHT / 4 - 1) * 2 + 1;
        break;
    case 1:
        pos[0] = getLandm(0, MAP_WIDTH / 4 - 1) * 2 + 1;
        pos[1] = getLandm(0, MAP_HEIGHT / 4 - 1) * 2 + MAP_HEIGHT / 2;
        break;
    case 2:
        pos[0] = getLandm(0, MAP_WIDTH / 4 - 1) * 2 + MAP_WIDTH / 2;
        pos[1] = getLandm(0, MAP_HEIGHT / 4 - 1) * 2 + MAP_HEIGHT / 2;
        break;
    case 3:
        pos[0] = getLandm(0, MAP_WIDTH / 4 - 1) * 2 + MAP_WIDTH / 2;
        pos[1] = getLandm(0, MAP_HEIGHT / 4 - 1) * 2 + 1;
        break;
    default:
        break;
    }
}
