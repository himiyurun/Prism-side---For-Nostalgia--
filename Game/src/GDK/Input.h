#ifndef _INPUT_H_
#define _INPUT_H_
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Input
{
    GLFWwindow* const* window;

    GLint KeyStat;

    bool swapStat;

    GLint MouseStat;

    GLfloat ShapePosBeg[2];

    GLfloat MousePosBeg[2];

public:

    Input(GLFWwindow* const* window)
        : window(window)
        , KeyStat(GLFW_RELEASE)
        , MouseStat(GLFW_RELEASE)
        , swapStat(false)
    {}

    virtual void excuteInput() const
    {
        if(swapStat)
            glfwPollEvents();
        else
            glfwWaitEvents();
    }

    virtual void swapInput()
    {
        if(swapStat)
        {
            swapStat = false;
        } else {
            swapStat = true;
        }
    }

    GLboolean getKey(int key) const { return glfwGetKey(*window, key); }

    virtual void setShapePosBeg(float position[2])
    {
        ShapePosBeg[0] = position[0];
        ShapePosBeg[1] = position[1];
    }

    virtual void setMousePosBeg(float size[2])
    {
        double xb, yb;
        glfwGetCursorPos(*window, &xb, &yb);
        MousePosBeg[0] = ((static_cast<float>(xb) / size[0]) * 2.0f - 1.0f);
        MousePosBeg[1] = (1.0f - (static_cast<float>(yb) / size[1]) * 2.0f);
    }

    virtual void getMousePos(float size[2], float position[2]) const
    {
        double xb, yb;
        glfwGetCursorPos(*window, &xb, &yb);
        position[0] = ((static_cast<float>(xb) / size[0]) * 2.0f - 1.0f);
        position[1] = (1.0f - (static_cast<float>(yb) / size[1]) * 2.0f);
    }

    virtual void getMousePosDiff(float size[2], float position[2]) const
    {
        float MousePos[2];
        getMousePos(size, MousePos);
        position[0] = MousePos[0] - MousePosBeg[0];
        position[1] = MousePos[1] - MousePosBeg[1];
    }

    virtual void getShapeMousePos(float size[2], float location[2]) const
    {
        float position[2];
        getMousePos(size, position);
        getMousePosDiff(size, position);
        location[0] = ShapePosBeg[0] + position[0];
        location[1] = ShapePosBeg[1] + position[1];
    }

    virtual void setKeyStat(int action)
    {
        KeyStat = action;
    }

    virtual void setMouseStat(int action)
    {
        MouseStat = action;
    }

    GLint getKeystat() const { return KeyStat; }

    GLint getMouseStat() const { return MouseStat; }

    //const GLfloat *(getMousePos() const) { return MousePos; }
};

#endif // _INPUT_H_