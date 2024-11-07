#include "Shader.h"


Shader::Shader(const char* vsrc, const char* fsrc, const void (*SetUp)(const float[2], float, const float[2], int, float, const int[5]))
    : program(createProgram(vsrc, fsrc))
    , setValue(SetUp)
    {
        std::cout<<program<<"create"<<std::endl;
    }

Shader::~Shader()
{
    if(program!=0)
    {
        glDeleteProgram(program);
    }
}

GLuint Shader::createProgram(const char *vert, const char *frag)
{
    const GLuint shader(glCreateProgram());
    if(vert != nullptr)
    {
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        readSource file(vert);
        if(file.getStat())
        {
            const char* bsrc(file.getsrc());
            glShaderSource(vobj, 1, &bsrc, NULL);
            glCompileShader(vobj);
            if(getShaderInfoLog(vobj, "Vertex Shader"))
                glAttachShader(shader, vobj);
        }
        glDeleteShader(vobj);
    }

    if(frag != nullptr)
    {
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        readSource file(frag);
        if(file.getStat())
        {
            const char* bsrc(file.getsrc());
            glShaderSource(fobj, 1, &bsrc, NULL);
            glCompileShader(fobj);
            if(getShaderInfoLog(fobj, "Fragment Shader"))
                glAttachShader(shader, fobj);
        }
        glDeleteShader(fobj);
    }

    glBindAttribLocation(shader, 0, "position");
    glBindFragDataLocation(shader, 0, "fragment");
    glLinkProgram(shader);

    if(getProgramInfoLog(shader))
        return shader;
    
    glDeleteProgram(shader);
    return 0;
}

GLboolean Shader::getShaderInfoLog(GLuint shader, const char *name)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(!status)
        std::cerr<<name<<" compile is Failed!!"<<std::endl;

    GLsizei bufSiz;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSiz);

    if(bufSiz > 1)
    {
        std::vector<GLchar> Log(bufSiz);
        GLsizei length;
        glGetShaderInfoLog(shader,bufSiz,&length,Log.data());
        std::cerr<<&Log[0]<<std::endl;
    }

    return static_cast<GLboolean>(status);
}

GLboolean Shader::getProgramInfoLog(GLuint shader)
{
    GLint status;
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    if(!status)
        std::cerr<<"Program compile Failed!!"<<std::endl;

    GLsizei bufSiz;
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &bufSiz);

    if(bufSiz > 1)
    {
        std::vector<GLchar> Log(bufSiz);
        GLsizei length;
        glGetProgramInfoLog(shader, bufSiz, &length, Log.data());
        std::cerr<<&Log[0]<<std::endl;
    }

    return static_cast<GLboolean>(status);
}

GLboolean (Shader::getsrc(const char* name, std::vector<GLchar> &src) const)
{
    std::ifstream file;

    file.open(name, std::ios::binary);
    if(file.fail())
    {
        std::cerr<<"Can't open "<<name<<std::endl;
        file.close();
        return false;
    }

    file.seekg(0L, std::ios::end);
    size_t bufSize(static_cast<size_t>(file.tellg()));
    file.seekg(0L, std::ios::beg);
    src.resize(bufSize + 1);
    file.read(src.data(), bufSize);
    src[bufSize] = '\0';
    if(file.fail())
    {
        std::cerr<<"Can't read "<<name<<std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}