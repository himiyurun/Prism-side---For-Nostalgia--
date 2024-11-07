#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <iostream>
#include <cstdlib>

#include "../Game.h"

#define MIN_SIZE    (0x00)
#define MAX_SIZE    (0x01)
#define SPECIFY     (0x02)


class Window : public Game
{
    GLFWwindow *const window;

    GLfloat size[2];

    GLfloat scale;
/*, const char* vsrc, const char* fsrc, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index, float locX, float locY, const Object::Vertex *uv, const char* imgname*/

public:
    //  コンストラクタ・・・ウインドウの作成、GLADの初期化
    //  width : ウィンドウの横幅
    //  height : ウインドウの高さ
    //  title : ウインドウの名前
    //  size : ゲームの次元
    //  param : スケールの基準の大きさ
    //  sscale : スケールの基準の大きさの指定（カスタムモードの場合）
    Window(int width, int height, const char* title, GLint size, GLenum param, float sscale);

    ~Window();

    explicit operator bool()
    {

        KeyInputProcess(size, scale);

        if(input->getMouseStat() == GLFW_PRESS)
        {
            MouseInputProcess(size, scale);
        }
        
        //input->swapInput();
        glfwPollEvents();

        if(enemy->IntersectPlayer(location, player->getVertex(), size, scale))
        {
            screenIndex = 0;
        }

        //UpdatePlayerPath(location, size, scale);

        return !glfwWindowShouldClose(window);
    }

    //  ウインドウのサイズが変更されたときに呼ばれる関数
    static void resize(GLFWwindow* window, int width, int height)
    {
        int fbwidth,
            fbheight;
        glfwGetFramebufferSize(window, &fbwidth, &fbheight);

        Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if(instance != nullptr)
        {
            instance->size[0] = fbwidth;
            instance->size[1] = fbheight;
            glViewport(0, 0, fbwidth, fbheight);
        }else{
            std::cerr<<"window instance is NULL"<<std::endl;
        }
    }

    //  キーボードが押されたときに呼ばれる関数
    static void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window *const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if(instance != nullptr)
        {
            //  キーボードの状態を保存
            instance->input->setKeyStat(action);
        }else{
            std::cerr<<"window instance is NULL"<<std::endl;
        }
    }

    static void button(GLFWwindow* window, int button, int action, int mods)
    {
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if(instance != nullptr)
        {
            instance->input->setMouseStat(action);
            float position[2];
            instance->input->getMousePos(instance->size, position);

            if( -0.5f * instance->scale / instance->size[0] + instance->location[0] < *(position) && 
                *(position) < 0.5f * instance->scale / instance->size[0] + instance->location[0] &&
                -0.5f * instance->scale / instance->size[1] + instance->location[1] < *(position+1) && 
                *(position+1) < 0.5f * instance->scale / instance->size[1] + instance->location[1])
            {
                instance->moveStat = true;
                instance->input->setMousePosBeg(instance->size);
                instance->input->setShapePosBeg(instance->location);
            }
        }else{
            std::cerr<<"window instance is NULL"<<std::endl;
        }
    }

    void drawPlayer()
    {
        if(screenIndex)
            player->drawCall(size, scale);
    }

    void swapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    void clearColor() const
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    const GLfloat *(getSize() const)  { return size; }

    GLfloat getScale() const { return scale; }

    const GLfloat *(getLocation() const) { return location; }
};

#endif //   WINDOW_H_