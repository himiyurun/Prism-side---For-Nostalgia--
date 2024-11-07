#include "Structure.h"

Structure::Structure(size_t vertexcount, const Object::Vertex *rect, float location[2], const char *imgname, const Object::Vertex *uv, GLsizei indexcount, const GLuint *index)
    : shape(new ShapeIndex(2, vertexcount, rect, indexcount, index))
    , texture(new Texture(2, vertexcount, uv, imgname))
    , rect(rect)
    , vertexcount(vertexcount)
    , location{ location[0], location[1] }
    , imgname(imgname)
    , uv(uv)
    , shaderindex(0)
    {
        float min[2]{ 0.0f, 0.0f };
        for(int i = 0; i < vertexcount; i++)
        {
            
            for(int j = 0; j < 2; j++)
            {
                if(rect[i].position[j] < min[j])
                    min[j] = rect[i].position[j];
            }
            
        }
        

        float max[2]{ 0.0f, 0.0f };
        for(int i = 0; i < vertexcount; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(rect[i].position[j] > max[j])
                    max[j] = rect[i].position[j];
            }
        }

        intersect[0].position[0] = min[0];
        intersect[0].position[1] = min[1];
        intersect[1].position[0] = max[0];
        intersect[1].position[1] = max[1];
    }

    Structure::~Structure()
    {
    }

bool Structure::InterSect(const float playerLoc[2], const Object::Vertex *playerRect, float size[2], float scale) const
{
    /*
    if( intersect[0].position[0] * scale / size[0] + location[0] < 0.5f * scale / size[0] + playerLoc[0] &&
        intersect[1].position[0] * scale/ size[0] + location[0] > -0.5f * scale / size[0] + playerLoc[0] &&
        intersect[0].position[1] * scale / size[1] + location[1] < 0.5f * scale / size[1] + playerLoc[1] &&
        intersect[1].position[1] * scale/ size[1] + location[1] > -0.5f * scale / size[1] + playerLoc[1])
    {
        std::cout<<intersect[0].position[0]<<", "<<location[0]<<", "<<playerLoc[0]<<std::endl;
        return true;
    }
    */
    if( rect[2].position[0] * scale / size[0] + location[0] < 0.4f * scale / size[0] + playerLoc[0] &&
        rect[0].position[0] * scale/ size[0] + location[0] > -0.4f * scale / size[0] + playerLoc[0] &&
        rect[2].position[1] * scale / size[1] + location[1] < 0.4f * scale / size[1] + playerLoc[1] &&
        rect[0].position[1] * scale/ size[1] + location[1] > -0.4f * scale / size[1] + playerLoc[1])
    {
        //std::cout<<"rect[2] : "<<rect[2].position[0] * scale / size[0] + location[0]<<", "<<rect[2].position[1] * scale / size[1] + location[1]<<", rect[0] : "<<rect[0].position[0] * scale / size[0] + location[0]<<", "<<rect[0].position[1] * scale / size[1] + location[1]<<", playerLoc : "<<playerLoc[0]<<", "<<playerLoc[1]<<std::endl;
        return true;
    }
    return false;
}
