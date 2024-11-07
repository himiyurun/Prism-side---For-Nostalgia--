#include "Enemy.h"

Enemy::Enemy(const char *vsrc, const char *fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, const float location[2], float positionld[2], float positionru[2], const Object::Vertex *uv, const char *imgname, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
    : shader(vsrc, fsrc, SetUp)
    , vertexcount(vertexcount)
    , vertex(vertex)
    , position{ location[0], location[1] }
    , positiontip{ { positionld[0], positionld[1] }, { positionru[0], positionru[1] } }
    , shape(2, vertexcount, vertex, indexcount, index)
    , texture(2, vertexcount, uv, imgname)
    , exist_stat(true)
    , counter(0)
{
}

Enemy::~Enemy()
{
}

bool Enemy::searchPlayer(std::vector<Points> &point, const int playerLoci[2], const float playerLoc[2], float size[2], float scale)
{   
    float pos[2]{};
    float div(15.f / WIDTH);
    for(int i = 0; i < point.size() - 1; i++)
    {
        pos[0] = ((point[i + 1].position[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
        pos[1] = (7.5f - (point[i + 1].position[1] * div + (div / 2.f))) * scale / size[1];

        //std::cout<<"pos[0] : "<<pos[0]<<", pos[1] : "<<pos[1]<<std::endl;

        search.emplace_back(pos[0], pos[1], point[i].dir);
    }

    Points a = point.back();
    pos[0] = ((playerLoci[0] * div + (div / 2.f)) - 7.5f) * scale / size[0];
    pos[1] = (7.5f - (playerLoci[1] * div + (div / 2.f))) * scale / size[1];
    search.emplace_back(pos[0], pos[1], a.dir);
    return true;
}

bool Enemy::IntersectPlayer(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const
{
    if( vertex[2].position[0] * scale / size[0] + position[0] < 0.4f * scale / size[0] + playerLoc[0] &&
        vertex[0].position[0] * scale/ size[0] + position[0] > -0.4f * scale / size[0] + playerLoc[0] &&
        vertex[2].position[1] * scale / size[1] + position[1] < 0.4f * scale / size[1] + playerLoc[1] &&
        vertex[0].position[1] * scale/ size[1] + position[1] > -0.4f * scale / size[1] + playerLoc[1])
    {
        //std::cout<<"rect[2] : "<<rect[2].position[0] * scale / size[0] + location[0]<<", "<<rect[2].position[1] * scale / size[1] + location[1]<<", rect[0] : "<<rect[0].position[0] * scale / size[0] + location[0]<<", "<<rect[0].position[1] * scale / size[1] + location[1]<<", playerLoc : "<<playerLoc[0]<<", "<<playerLoc[1]<<std::endl;
        return true;
    }
    return false;
}

void Enemy::move(const float size[2], float scale, const float playerLoc[2])
{
    const float velocity = 0.02f;

    if(!search.empty() && counter < search.size()) {
        switch(search[counter].dir)
        {
        case UP:
            position[1] += velocity * scale / size[1];
            std::cout<<"UP"<<std::endl;
            if(position[1] > search[counter].position[1])
            {
                //std::cout<<"position[1] : "<<position[1]<<", search[counter].position[1] : "<<search[counter].position[1]<<std::endl;
                counter++;
            }
            break;
        case DOWN:
            position[1] -= velocity * scale / size[1];
            std::cout<<"DOWN"<<std::endl;
            if(position[1] < search[counter].position[1])
            {
                //std::cout<<"position[1] : "<<position[1]<<", search[counter].position[1] : "<<search[counter].position[1]<<std::endl;
                counter++;
            }
            break;
        case RIGHT:
            position[0] += velocity * scale / size[0];
            std::cout<<"RIGHT"<<std::endl;
            if(position[0] > search[counter].position[0])
            {
                //std::cout<<"position[0] : "<<position[0]<<", search[counter].position[0] : "<<search[counter].position[0]<<std::endl;
                counter++;
            }
            break;
        case LEFT:
            position[0] -= velocity * scale / size[0];
            std::cout<<"LEFT"<<std::endl;
            if(position[0] < search[counter].position[0])
            {
                //std::cout<<"position[0] : "<<position[0]<<", search[counter].position[0] : "<<search[counter].position[0]<<std::endl;
                counter++;
            }
            break;
        default:
            break;
        }
        //counter++;
        //std::cout<<"counter : "<<counter<<std::endl;
    }
    

    //position[1] += velocity;
}
