#ifndef _READSOURCE_H_
#define _READSOURCE_H_

#include <iostream>
#include <vector>
#include <fstream>

class readSource
{
    const char* srcname;

    std::vector<char> src;

    bool readStat;

public:

    readSource(const char* srcname)
        : srcname(srcname)
        , readStat(true)
    {
        if(srcname != nullptr)
        {
            std::ifstream file;
            file.open(srcname, std::ios::binary);
            if(file.fail())
            {
                std::cerr<<"Can't Open "<<srcname<<std::endl;
                readStat = false;
            }

            if(readStat)
            {
                file.seekg(0L, std::ios::end);
                size_t bufsiz(file.tellg());
                file.seekg(0L, std::ios::beg);
                src.resize(bufsiz + 1);
                file.read(src.data(), bufsiz);
                src[bufsiz] = '\0';
                if(file.fail())
                {
                    std::cerr<<"Can't read "<<srcname<<std::endl;
                    readStat = false;
                }
                file.close();
            }
        }
    }

    const char* getsrc() const { return src.data(); }

    bool getStat() const { return readStat; }
};

#endif