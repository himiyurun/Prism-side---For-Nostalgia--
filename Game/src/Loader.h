#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <vector>

class Loader
{
    std::vector<std::vector<int>> strindex;

    int tilePos;

    const int tilesize[2];

public:

    Loader(int firstPos)
        : tilePos(firstPos)
        , tilesize{ 2, 2 }
    {
        strindex.resize(4);
        strindex[0].push_back(1);
        strindex[1].push_back(3);
        strindex[2].push_back(0);
        strindex[3].push_back(2);
    }

    void setIndex(const int *index, size_t count, int tileHWD)
    {
        strindex.push_back(std::vector<int>());
        strindex[tileHWD].resize(count);

        for(int i = 0; i < count; i++)
        {
            strindex[tileHWD][i] = *(index+i);
        }
    }

    bool chengeTile(float location[2])
    {
        if( location[0] < -1.0f ||
            1.0f < location[0] ||
            location[1] < -1.0f ||
            1.0f < location[1] )
        {
            int lasPos = tilePos;
            float lasLoc[2]{ location[0], location[1] };
            if(location[0] > 1.0f)
            {
                tilePos+=1;
                location[0] = -1.0f;
            }
            else if(location [0] < -1.0f)
            {
                tilePos-=1;
                location[0] = 1.0f;
            }
            if(location[1] > 1.0f)
            {
                tilePos-=tilesize[0];
                location[1] = -1.0f;
            }
            else if(location [1] < -1.0f)
            {
                tilePos+=tilesize[0];
                location[1] = 1.0f;
            }

            if(tilePos >= 0 && tilePos < strindex.size())
            {
                return true;
            }
            location[0] = lasLoc[0];
            location[1] = lasLoc[1];
            tilePos = lasPos;
        }
        
        return false;
    }

    int getTileStrsize() const { return strindex[tilePos].size(); }

    int getStrHWD(int num) const { return strindex[tilePos][num]; }
};

#endif