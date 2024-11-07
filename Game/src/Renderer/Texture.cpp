//
// Created by rayrk on 24/09/01.
//

#include "Texture.h"

Texture::Texture(GLint size, const GLsizei uvcount, const Object::Vertex *uv, const char *imgname)
{
    if(!readBMP(imgname, img, bufSize)) {
        std::cerr<<"Can't read "<<imgname<<std::endl;
    }

    if(size != 0) {
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, uvcount * sizeof(Object::Vertex), uv, GL_STATIC_DRAW);

        glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
    }

    if(imgname) { 
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        //std::cout<<"tex : "<<tex<<std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufSize[0], bufSize[1], 0, GL_BGR, GL_UNSIGNED_BYTE, img.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}

bool Texture::readBMP(const char *imgname, std::vector<GLubyte> &img, int size[2]) const
{
    BITMAPFILEHEADER fileh;
    BITMAPINFOHEADER infoh;
    std::ifstream file;
    if(!imgname) {
        std::cerr<<"imgname is nullptr"<<std::endl;
        return false;
    }

    file.open(imgname, std::ios::binary);
    if (file.fail())
    {
        std::cerr << "file is not open" << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char *>(&fileh), sizeof(BITMAPFILEHEADER));
    file.read(reinterpret_cast<char *>(&infoh), sizeof(BITMAPINFOHEADER));

    if (file.fail())
    {
        std::cerr << "Can't read file" << std::endl;
        return false;
    }

    size[0] = infoh.biWidth;
    size[1] = infoh.biHeight;

    //std::cout<<"size : "<<size[0]<<" "<<size[1]<<std::endl;
    img.resize(size[0] * size[1] * 3);
    file.read(reinterpret_cast<char *>(img.data()), static_cast<size_t>(img.size()));

    file.close();
    return true;
}