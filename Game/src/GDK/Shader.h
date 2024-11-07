#ifndef _SHADER_H_
#define _SHADER_H_
#include <glad/glad.h>

#include "readSource.h"

class Shader
{
    const GLuint program;

    //  値を渡す関数
    //  size : ウインドウのサイズ
    //  sclae : ウインドウの拡大率
    //  location : オブジェクトの位置
    //  texIndex : テクスチャオブジェクトの番号
    //  Light : 放射の強さ
    const void (*setValue)(const float size[2], float scale, const float location[2], int texIndex, float Light, const int loc[5]);

public:

    //  コンストラクタ・・・シェーダープログラムの作成
    //  vsrc : バーテックスシェーダソースファイル
    //  fsrc : フラグメントシェーダソースファイル
    Shader(const char* vsrc, const char* fsrc, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]));

    //  デストラクタ
    ~Shader();

    virtual GLuint createProgram(const char* vert, const char* frag);

    virtual GLboolean (getsrc(const char* name, std::vector<GLchar> &src) const);

    virtual GLboolean getShaderInfoLog(GLuint shader, const char* name);

    virtual GLboolean getProgramInfoLog(GLuint shader);

    GLuint getProgram() const { return program; }

    void use() const
    {
        glUseProgram(program);
        //std::cout<<"ex"<<std::endl;
    }

    void setup(const float size[2], float scale, const float location[2], int texIndex, float Light, const int loc[5]) const
    {
        setValue(size, scale, location, texIndex, Light, loc);
    }

    GLint getUnfLoc(const char* name) const { return glGetUniformLocation(program, name); }

    GLint getAtrLoc(const char* name) const { return glGetUniformLocation(program, name); }
};

#endif // _SHADER_H_