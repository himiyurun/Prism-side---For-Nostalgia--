#include "Game.h"

const GLuint Game::mazeIndex[6] = {
    1, 0, 3, 1, 2, 3
};

Game::Game(GLFWwindow* const* window, const GLint size)
    : size(size)
    , input(new Input(window))
    , map(5.5f, 5.5f)
    , moveStat(false)
    , screenIndex(0)
    , location{ 0.0f, 0.0f }
    , direction(PLAYER_STOP)
    , astar(map.field, MAP_HEIGHT, MAP_WIDTH)
{
    map.createMap();

}

Game::~Game()
{
}

void Game::InitPlayer(const char *vsrc, const char *fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const Object::Vertex *uv, const char *imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
{
    player = std::make_unique<Player>(Player(vsrc, fsrc, size, vertexcount, vertex, indexcount, index, location, uv, imgname, SetUp));
    lastLocation[0] = 1;
    lastLocation[1] = 1;
}

void Game::InitMask()
{
    const int offset(1);

    Object::Vertex block[] = {
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT }
    };

    Object::Vertex mem = {15.f / MAP_WIDTH, 15.f / MAP_HEIGHT};
    constexpr float div = 15.f / 2.f;
    constexpr float uv_div = 1.0f / 8.0f;

    constexpr float uvpixel = 1.0f;
    Object::Vertex uv[4] = {
        {  uv_div * 3, uvpixel },
        {  uv_div * 2, uvpixel },
        {  uv_div * 2, uv_div * 7 + 0.0001f },
        {  uv_div * 3, uv_div * 7 + 0.0001f }
    };

    /*
        {  uv_div, uvpixel },
        {  0.0f, uvpixel },
        {  0.0f, uv_div * 7 + 0.0001f },
        {  uv_div, uv_div * 7 + 0.0001f }

        {  uv_div * 3, uvpixel },
        {  uv_div * 2, uvpixel },
        {  uv_div * 2, uv_div * 7 + 0.0001f },
        {  uv_div * 3, uv_div * 7 + 0.0001f }
    */

    constexpr GLfloat begLocation[2]{ -div, -div };
    GLfloat location[2]{ 0.f, 0.f };

    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            block[0] = {  (mem.position[0] * (j + 1)) - div,    div - (mem.position[1] * i) };
            block[1] = {  (mem.position[0] * j) - div,          div - (mem.position[1] * i) };
            block[2] = {  (mem.position[0] * j) - div,          div - (mem.position[1] * (i + 1)) };
            block[3] = {  (mem.position[0] * (j + 1)) - div,    div - (mem.position[1] * (i + 1)) };

            /*
            std::cout<<"block[0] : "<<block[0].position[0]<<", "<<block[0].position[1]<<std::endl;
            std::cout<<"block[1] : "<<block[1].position[0]<<", "<<block[1].position[1]<<std::endl;
            std::cout<<"block[2] : "<<block[2].position[0]<<", "<<block[2].position[1]<<std::endl;
            std::cout<<"block[3] : "<<block[3].position[0]<<", "<<block[3].position[1]<<std::endl;
            std::cout<<std::endl;
            */
            //mazeRect.emplace_back(block, uv);
            
            mask.emplace_back(4, block, location, "src/objpre.bmp", uv, 6, mazeIndex);
        }
    }
}

void Game::InitEnemy(const float size[2], float scale, const char *vsrc, const char *fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float location[2], float positionld[2], float positionru[2], const Object::Vertex *uv, const char *imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
{
    enemy = std::make_unique<Enemy>(Enemy(vsrc, fsrc, vertexcount, vertex, indexcount, index, location, positionld, positionru, uv, imgname, SetUp));
    float buf[2]{ 0.f, 0.f };
    int pos[2]{ map.getLandm(0, MAP_WIDTH / 2 - 1) * 2 + 1, map.getLandm(0, MAP_HEIGHT / 2 - 1) * 2 + 1 };
    map.setStart(buf, pos[0], pos[1], const_cast<float*>(size), scale);
    enemy->setPos(buf);

    astar.searchPath(pos[0], pos[1], 1, 1);
    map.drawField();

    int PlayerLoci[2]{ map.calcTilex(0.0f, this->location[0], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale), map.calcTiley(0.0f, this->location[1], 15.f / 2.f, 15.f / MAP_HEIGHT, size, scale) };
    std::cout<<"PlayerLoci[0] : "<<PlayerLoci[0]<<", PlayerLoci[1] : "<<PlayerLoci[1]<<std::endl;
    enemy->searchPlayer(astar.stopover, PlayerLoci, location, const_cast<float*>(size), scale);
}

void Game::searchPlayerPath(std::vector<Points> &point, const float playerLoc[2], float size[2], float scale)
{
    //enemy->searchPlayer(point, location, size, scale);
}

void Game::UpdatePlayerPath(const float playerLoc[2], float size[2], float scale)
{
    searchPos search = enemy->search.back();
    const float div(15.f / MAP_WIDTH);
    int EnemyPos[2]{ map.calcTilex(0.0f, search.position[0], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale), map.calcTiley(0.0f, search.position[1], 15.f / 2.f, 15.f / MAP_HEIGHT, size, scale) };
    int nowPos[2]{ map.calcTilex(0.0f, location[0], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale), map.calcTiley(0.0f, location[1], 15.f / 2.f, 15.f / MAP_HEIGHT, size, scale) };
    if(nowPos[0] != EnemyPos[0] || nowPos[1] != EnemyPos[1])
    {
        searchPos search = enemy->search.back();
        std::cout<<"nowPos[0] : "<<nowPos[0]<<", nowPos[1] : "<<nowPos[1]<<std::endl;
        std::cout<<"EnemyPos[0] : "<<EnemyPos[0]<<", EnemyPos[1] : "<<EnemyPos[1]<<std::endl;
        switch(search.dir)
        {
        case UP:
            if(nowPos[1] > EnemyPos[1])
            {
                enemy->search[enemy->search.size() - 1].position[0] = ((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
                enemy->search[enemy->search.size() - 1].position[1] = (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1];

                if(nowPos[0] == map.calcTilex(0.0f, enemy->search[enemy->search.size() - 2].position[0], 15.f / 2.f, div, size, scale) && nowPos[1] == map.calcTiley(0.0f, enemy->search[enemy->search.size() - 2].position[1], 15.f / 2.f, div, size, scale))
                {
                    if(enemy->getCount() == enemy->search.size())
                    {
                        enemy->setCounter(enemy->search.size() - 2);
                    }
                    enemy->search.pop_back();
                }
                return;
            }
            break;
        case DOWN:
            if(nowPos[1] < EnemyPos[1])
            {
                enemy->search[enemy->search.size() - 1].position[0] = ((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
                enemy->search[enemy->search.size() - 1].position[1] = (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1];

                if(nowPos[0] == map.calcTilex(0.0f, enemy->search[enemy->search.size() - 2].position[0], 15.f / 2.f, div, size, scale) && nowPos[1] == map.calcTiley(0.0f, enemy->search[enemy->search.size() - 2].position[1], 15.f / 2.f, div, size, scale))
                {
                    if(enemy->getCount() == enemy->search.size())
                    {
                        enemy->setCounter(enemy->search.size() - 2);
                    }
                    enemy->search.pop_back();
                }
                /*
                もし現在の位置とenemyPosが一緒だと方向が更新されずに前の方向に進んで膠着するので以下のような条件分岐が必要
                const float *buf = enemy->getLocPointer();
                int bufi(map.calcTiley(0.0f, buf[1], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale));
                if(EnemyPos[1] == bufi)
                {
                    enemy->search[enemy->search.size() - 1].dir = UP;
                }
                */
                return;
            }
            break;
        case LEFT:
            if(nowPos[0] > EnemyPos[0])
            {
                enemy->search[enemy->search.size() - 1].position[0] = ((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
                enemy->search[enemy->search.size() - 1].position[1] = (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1];

                if(nowPos[0] == map.calcTilex(0.0f, enemy->search[enemy->search.size() - 2].position[0], 15.f / 2.f, div, size, scale) && nowPos[1] == map.calcTiley(0.0f, enemy->search[enemy->search.size() - 2].position[1], 15.f / 2.f, div, size, scale))
                {
                    if(enemy->getCount() == enemy->search.size())
                    {
                        enemy->setCounter(enemy->search.size() - 2);
                    }
                    enemy->search.pop_back();
                }
                return;
            }
            break;
        case RIGHT:
            if(nowPos[0] < EnemyPos[0])
            {
                enemy->search[enemy->search.size() - 1].position[0] = ((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
                enemy->search[enemy->search.size() - 1].position[1] = (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1];

                if(nowPos[0] == map.calcTilex(0.0f, enemy->search[enemy->search.size() - 2].position[0], 15.f / 2.f, div, size, scale) && nowPos[1] == map.calcTiley(0.0f, enemy->search[enemy->search.size() - 2].position[1], 15.f / 2.f, div, size, scale))
                {
                    if(enemy->getCount() == enemy->search.size())
                    {
                        enemy->setCounter(enemy->search.size() - 2);
                    }
                    enemy->search.pop_back();
                }
                return;
            }
            break;
        }

        if(nowPos[0] == EnemyPos[0] && nowPos[1] < EnemyPos[1])
        {
            std::cout<<"up"<<std::endl;
            enemy->search.emplace_back(((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0], (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1], UP);
            std::cout<<"nowPos[0] : "<<nowPos[0]<<", nowPos[1] : "<<nowPos[1]<<std::endl;
            std::cout<<"EnemyPos[0] : "<<EnemyPos[0]<<", EnemyPos[1] : "<<EnemyPos[1]<<std::endl;
            std::cout<<"enemy search position[0] : "<<enemy->search[enemy->search.size() - 1].position[0]<<", position[1] : "<<enemy->search[enemy->search.size() - 1].position[1]<<std::endl;

        }
        else if(nowPos[0] == EnemyPos[0] && nowPos[1] > EnemyPos[1])
        {
            std::cout<<"down"<<std::endl;
            enemy->search.emplace_back(((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0], (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1], DOWN);
            std::cout<<"nowPos[0] : "<<nowPos[0]<<", nowPos[1] : "<<nowPos[1]<<std::endl;
            std::cout<<"EnemyPos[0] : "<<EnemyPos[0]<<", EnemyPos[1] : "<<EnemyPos[1]<<std::endl;
            std::cout<<"enemy search position[0] : "<<enemy->search[enemy->search.size() - 1].position[0]<<", position[1] : "<<enemy->search[enemy->search.size() - 1].position[1]<<std::endl;

        }
        if(nowPos[0] > EnemyPos[0] && nowPos[1] == EnemyPos[1])
        {
            std::cout<<"right"<<std::endl;
            enemy->search.emplace_back(((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0], (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1], RIGHT);
        }
        else if(nowPos[0] < EnemyPos[0] && nowPos[1] == EnemyPos[1])
        {
            std::cout<<"left"<<std::endl;
            enemy->search.emplace_back(((nowPos[0] * div + (div / 2.f)) - 7.5f) * scale / size[0], (7.5f - (nowPos[1] * div + (div / 2.f))) * scale / size[1], LEFT);
        }
        //std::cout<<"enemy->search.size() : "<<enemy->search.size()<<std::endl;
    }
    lastLocation[0] = nowPos[0];
    lastLocation[1] = nowPos[1];
}

bool Game::GameOver(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const
{
    if(enemy->IntersectPlayer(playerLoc, playerRect, size, scale))
    {
        return true;
    }
    return false;
}

void Game::convertMapStr(GLuint offset, const float size[2], float scale)
{
    Object::Vertex block[] = {
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT },
        {  11.f / MAP_WIDTH,  11.f / MAP_HEIGHT }
    };

    Object::Vertex mem = {15.f / MAP_WIDTH, 15.f / MAP_HEIGHT};
    constexpr float div = 15.f / 2.f;
    constexpr float uv_div = 1.0f / 8.0f;

    constexpr float uvpixel = 1.0f;
    Object::Vertex uv[4] = {
        {  uv_div, uvpixel },
        {  0.0f, uvpixel },
        {  0.0f, uv_div * 7 + 0.0001f },
        {  uv_div, uv_div * 7 + 0.0001f }
    };

    constexpr GLfloat begLocation[2]{ -div, -div };
    GLfloat location[2]{ 0.f, 0.f };

    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            switch(map.field[i][j])
            {
            case MAP_WALL:
                block[0] = {  (mem.position[0] * (j + 1)) - div,    div - (mem.position[1] * i) };
                block[1] = {  (mem.position[0] * j) - div,          div - (mem.position[1] * i) };
                block[2] = {  (mem.position[0] * j) - div,          div - (mem.position[1] * (i + 1)) };
                block[3] = {  (mem.position[0] * (j + 1)) - div,    div - (mem.position[1] * (i + 1)) };

                /*
                std::cout<<"block[0] : "<<block[0].position[0]<<", "<<block[0].position[1]<<std::endl;
                std::cout<<"block[1] : "<<block[1].position[0]<<", "<<block[1].position[1]<<std::endl;
                std::cout<<"block[2] : "<<block[2].position[0]<<", "<<block[2].position[1]<<std::endl;
                std::cout<<"block[3] : "<<block[3].position[0]<<", "<<block[3].position[1]<<std::endl;
                std::cout<<std::endl;
                */
                /*
                uv[1] = { uvpixel * 1,  0.f };
                uv[2] = { uvpixel * 1,  uvpixel * 1 };
                uv[3] = {  0.f,  uvpixel * 1 };
                */
                mazeRect.emplace_back(block, uv);
                
                //std::cout<<"num : "<<mazeRect.size() - 1<<std::endl;
                screen[offset].structure.push_back(Structure(4, mazeRect[mazeRect.size() - 1].vertex, location, "src/objpre.bmp", mazeRect[mazeRect.size() - 1].uv, 6, mazeIndex));
                //intersect[offset].Rigist(&screen[offset].structure.back(), screen[offset].structure.size() - 1, block[2].position[0], block[2].position[1], block[0].position[0], block[0].position[1]);
                //std::cout<<"num : "<<i*MAP_WIDTH + j<<std::endl;
                break;
            case MAP_ROAD:
                break;
            case MAP_OBJECT_TABLE:
                break;
            case MAP_OBJECT_RAMP:
                break;
            default:
                break;
            }
        }
    }
    
    int siz = 0;
    for(int i = 0; i <DIV_HEIGHT; i++)
    {
        for(int j = 0; j <DIV_WIDTH; j++)
        {
            //std::cout<<intersect[offset].array[i][j].size()<<",";
            siz += intersect[offset].array[i][j].size();
        }
    }
    //std::cout<<"size : "<<siz<<std::endl;
}

void Game::InitMapRect(Object::Vertex vertex[4], Object::Vertex uv[4]) const
{
    vertex[0] = {  5.5f / MAP_WIDTH,  5.5f / MAP_HEIGHT };
}

void Game::createScreen(const char *name, GLfloat insecPos[2], GLfloat insecSize[2])
{
    screen.push_back(Screen(name));
    intersect.emplace_back(insecPos, insecSize);
}

void Game::setTexture(const char *imgname)
{
    str_texture.emplace_back(0, 0, nullptr, imgname);
}

//  setShader in the screen class
//  scroffset : スクリーンのインデックスを指定。NULLの場合は screenIndex の値が割り当てられる
//  vsrc : バーテックスシェーダソースファイル名
//  fsrc : フラグメントシェーダソースファイル名
void Game::setShader(const char* vsrc, const char* fsrc, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
{
    shader.emplace_back(vsrc, fsrc, SetUp);
}

//  シェーダーに値を渡す
//  size : ウインドウのサイズ
//  sclae : ウインドウの拡大率
//  location : オブジェクトの位置
//  texIndex : テクスチャオブジェクトの番号　0未満の値を入力すると無視する
//  Light : 放射の強さ　0未満の値を入力すると無視する
void Game::excuteShader(GLsizei shaderIndex, const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const
{   
    shader[shaderIndex].use();

    shader[shaderIndex].setup(size, scale, location, texIndex, Light, loc);

    //std::cout<<"ex"<<std::endl;
}

void Game::setUI(GLint offset, const char *vsrc, const char *fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locx, float locy, const char *imgname, const Object::Vertex *uv, const void (*UIfunc)(int*, float[2], float), const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
{
    screen[offset].ui.push_back(UI(vsrc, fsrc, size, vertexcount, vertex, indexcount, index, locx, locy, imgname, uv, UIfunc, SetUp));
}

void Game::setStructure(GLint offset, GLsizei vertexcount, const Object::Vertex *vertex, GLfloat location[2], const char* imgname, const Object::Vertex* uv, GLsizei indexcount, const GLuint *index, const float size[2], float scale)
{
    screen[offset].structure.push_back(Structure(vertexcount, vertex, location, imgname, uv, indexcount, index));
    intersect[offset].Rigist(&screen[offset].structure.back(), screen[offset].structure.size() - 1, -0.5f * scale / size[0] + location[0], -0.5f * scale / size[1] + location[1], 0.5f * scale / size[0] + location[0], 0.5f * scale / size[1] + location[1]);
}

void Game::drawTile(GLint locationLoc, const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const
{
    /*
    for(int i = 0; i < screen[screenIndex].loader->getTileStrsize(); i++)
    {
        excuteShader(screen[screenIndex].structure[screen[screenIndex].loader->getStrHWD(i)].getShaderIndex(), size, scale, screen[screenIndex].structure[screen[screenIndex].loader->getStrHWD(i)].getStrLocation(), texIndex, Light, loc);
        //glUniform2fv(locationLoc, 1, screen[screenIndex].structure[screen[screenIndex].loader->getStrHWD(i)].getStrLocation());
        screen[screenIndex].structure[screen[screenIndex].loader->getStrHWD(i)].active(++i);
    }
    */
    int i = 0;
    for(Structure a : screen[screenIndex].structure)
    {
        excuteShader(a.getShaderIndex(), size, scale, a.getStrLocation(), texIndex, Light, loc);

        
        glActiveTexture(str_texture[0].getTex());
        str_texture[0].bind();
        glUniform1i(loc[3], str_texture[0].getTex());

        a.active(0);
    }
}

void Game::actvtex(GLsizei strnum, int8_t handle) const
{
    screen[screenIndex].structure[strnum].active(handle);
}

void Game::darwUI(GLsizei num, float size[2], float scale, int loc[5]) const
{
    if(!screenIndex)
    screen[screenIndex].ui[num].drawUI(size, scale, loc);
}

void Game::moveEnemy(const float size[2], float scale, const float playerLoc[2])
{
    if(!screenIndex) return;
    enemy->move(size, scale, location);
}

void Game::drawEnemy(const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const
{
    if(screenIndex)
        enemy->active(&texIndex, size, scale, location, texIndex, Light, loc);
}

void Game::drawMask(const float size[2], float scale, const float location[2], int texIndex, float Light, int loc[5]) const
{
    if(screenIndex) {
        int i = 0;
        for(const auto a : mask)
        {
            if(!map.getStat(i % MAP_WIDTH, i / MAP_WIDTH))
            {
                excuteShader(a.getShaderIndex(), size, scale, a.getStrLocation(), texIndex, Light, loc);

                glActiveTexture(str_texture[0].getTex());
                str_texture[0].bind();
                glUniform1i(loc[3], str_texture[0].getTex());

                a.active(0);
            }
            i++;
        }
    }
}

void Game::excuteUI(GLsizei num, float mospos[2], float size[2], float scale)
{
    if(input->getMouseStat() != GLFW_RELEASE)
        screen[screenIndex].ui[num].excuteUI(&screenIndex, mospos, size, scale);
}

void Game::KeyInputProcess(float size[2], float scale)
{
    if(GameOver(location, player->getVertex(), size, scale))
    {
        screenIndex = 0;
    }

    if(!screenIndex) return;

    GLfloat lastLoc[] = { location[0], location[1] };
    playerDrection dir = direction;
    static constexpr float velocity = 0.014f;
    if(input->getKey(GLFW_KEY_W)) {
        location[1]+=velocity * scale / size[1];
        direction = PLAYER_UP;
    } else if(input->getKey(GLFW_KEY_S)) {
        location[1]-=velocity * scale / size[1];
        direction = PLAYER_DOWN;
    } if(input->getKey(GLFW_KEY_D)) {
        location[0]+=velocity * scale / size[0];
        direction = PLAYER_RIGHT;
    } else if(input->getKey(GLFW_KEY_A)) {
        location[0]-=velocity * scale / size[0];
        direction = PLAYER_LEFT;
    }
    if(input->getKey(GLFW_KEY_0))
        screenIndex = 1;
    else if(input->getKey(GLFW_KEY_1))
        screenIndex = 0;

    intersect[screenIndex].locateTile(strIndex, -0.5f * scale / size[0] + location[0], -0.5f * scale / size[1] + location[1], 0.5f * scale / size[0] + location[0], 0.5f * scale / size[1] + location[1]);
    //std::cout<<"strIndex : "<<strIndex.size()<<std::endl;
    for(const auto a : strIndex)
    {
        //  スクリーンでのインデックスなため、スクリーンにない矩形は無視される
        //  スクリーン内のストラクチャーの数が一緒のため問題ないが、違う場合はメモリリークの危険性があるため要修正
        if(screen[screenIndex].structure[a].InterSect(player->getLocPointer(), player->getVertex(), size, scale))
        {
            location[0] = lastLoc[0];
            location[1] = lastLoc[1];
            direction = dir;
            break;
        }
    }

    /*
    if(screen[0].structure[7].InterSect(location, player->getVertex(), size, scale))
    {
        location[0] = lastLoc[0];
        location[1] = lastLoc[1];
    }
    */

    if(map.collision(location, player->getVertex(), size, scale))
    {
        location[0] = lastLoc[0];
        location[1] = lastLoc[1];
        //direction = dir;
    }

    /*
    if(screen[screenIndex].loader->chengeTile(location))
    {

    }
    */
    if(input->getKeystat() == GLFW_PRESS) {
        if(input->getKey(GLFW_KEY_Q))
        {
            int pos[2]{ map.calcTilex(0.0f, location[0], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale), map.calcTiley(0.0f, location[1], 15.f / 2.f, 15.f / MAP_HEIGHT, size, scale) };
            if(map.getStat(pos[0], pos[1]))
            {
            switch(direction)
            {
                case PLAYER_UP:
                    map.rigistClear(pos[0], pos[1] - 1);
                    break;
                case PLAYER_DOWN:
                    map.rigistClear(pos[0], pos[1] + 1);
                    break;
                case PLAYER_LEFT:
                    map.rigistClear(pos[0] - 1, pos[1]);
                    break;
                case PLAYER_RIGHT:
                    map.rigistClear(pos[0] + 1, pos[1]);
                    break;
                case PLAYER_STOP:
                    break;
                default:
                    break;
                }
            } else {
                map.rigistClear(pos[0], pos[1]);
            }
        } else if(input->getKey(GLFW_KEY_E))
        {
            int pos[2]{ map.calcTilex(0.0f, location[0], 15.f / 2.f, 15.f / MAP_WIDTH, size, scale), map.calcTiley(0.0f, location[1], 15.f / 2.f, 15.f / MAP_HEIGHT, size, scale) };
            switch(direction)
            {
                case PLAYER_UP:
                    pos[1] -= 2;
                    break;
                case PLAYER_DOWN:
                    pos[1] += 2;
                    break;
                case PLAYER_LEFT:
                    pos[0] -= 2;
                    break;
                case PLAYER_RIGHT:
                    pos[0] += 2;
                    break;
                case PLAYER_STOP:
                    break;
                default:
                    break;
            }
            if(pos[0] >= 0 && pos[0] < MAP_WIDTH && pos[1] >= 0 && pos[1] < MAP_HEIGHT)
            {
                map.rigistClear(pos[0], pos[1]);
            }
        }

        if(input->getKey(GLFW_KEY_R))
        {
            map.rigistStar(0, 0);
        }
    }

    strIndex.clear();
}

void Game::MouseInputProcess(float size[2], float scale)
{
    if(moveStat)
    {
        //input->getShapeMousePos(size, location);
        if(!input->getMouseStat())
            moveStat = false;

        /*
        if(-1.0f > location[0] || 1.0f < location[0] || -1.0f > location[1] || 1.0f < location[1])
        {
            if(-1.0f > location[0])
                location[0] = -1.0f;
            else if(1.0f < location[0])
                location[0] = 1.0f;
            if(-1.0f > location[1])
                location[1] = -1.0f;
            else if(1.0f < location[1])
                location[1] = 1.0f;
        }
        */
    }

    float mospos[2];
    input->getMousePos(size, mospos);
    if(!screenIndex)
        excuteUI(0, mospos, size, scale);
}
